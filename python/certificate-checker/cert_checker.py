import os, sys, base64, textwrap
import cgi
import codecs
import json
import re
import smtplib
import time
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from datetime import datetime
from smtplib import SMTPException

import jks
import OpenSSL.crypto

""" requirement.txt
# cffi==1.8.3
# cryptography==1.5.2
# enum34==1.1.6
# idna==2.1
# ipaddress==1.0.17
# javaobj-py3==0.2.2
# pyasn1==0.1.9
# pyasn1-modules==0.0.8
# pycparser==2.14
# pycrypto==2.6.1
pyjks==0.5.1
pyOpenSSL==16.1.0
# six==1.10.0
# twofish==0.3.0
"""

class CertData(object):
    """ Certificate data structure """
    cert = None
    alias = None
    expiration_date = None
    days_left = None
    issuer = None

    def __init__(self):
        pass


class JksCertChecker(object):

    def __init__(self, filename, store_password=""):
        self.filename = filename
        self.store_password = store_password
        self.keystore = self._open_keystore(filename, store_password)

    def get_private_key_certs(self, alias, key_password):
        """ Get private keys' certificates (from certificate chain) """
        certs = []
        private_key = self.keystore.private_keys[alias]
        if not private_key:
            return certs
        elif not private_key.is_decrypted():
            private_key.decrypt(key_password)

        for cert_tuple in private_key.cert_chain:
            pem_string = self._print_pem(cert_tuple[1], "CERTIFICATE")
            x509_cert = OpenSSL.crypto.load_certificate(OpenSSL.crypto.FILETYPE_PEM, pem_string)
            cert_data = self._load_cert_data(alias, x509_cert)
            certs.append(cert_data)
        return certs

    def get_trusted_certs(self):
        """ Get trusted certificates """
        certs = []
        for cert_alias, x509_cert in self.keystore.certs.items():
            pem_string = self._print_pem(x509_cert.cert, "CERTIFICATE")
            x509_cert = OpenSSL.crypto.load_certificate(OpenSSL.crypto.FILETYPE_PEM, pem_string)
            cert_data = self._load_cert_data(cert_alias, x509_cert)
            certs.append(cert_data)
        return certs

    def _load_cert_data(self, alias, cert):
        # http://www.pyopenssl.org/en/stable/api/crypto.html#x509-objects
        # format: YYYYMMDDhhmmssZ
        exp_date = cert.get_notAfter()
        exp_date = datetime.strptime(exp_date[:8], "%Y%m%d").date()
        today = datetime.utcnow().date()

        data = CertData()
        data.cert = cert
        data.alias = alias
        data.expiration_date = exp_date
        data.days_left = (exp_date-today).days
        data.issuer = cert.get_issuer()
        return data

    def _open_keystore(self, filename, store_password):
        return jks.KeyStore.load(filename, store_password)

    def _print_pem(self, data, type):
        crt_text = ""
        crt_text += "-----BEGIN %s-----\n" % type
        crt_text += "\n".join(textwrap.wrap(base64.b64encode(data), 64))
        crt_text += "\n-----END %s-----" % type
        return crt_text


class EmailServer(object):
    """ Email Server
        * https://docs.python.org/2/library/smtplib.html
        * http://stackoverflow.com/questions/882712/sending-html-email-using-python
    """

    def __init__(self, smtp_hostname, from_email):
        self.smtp_hostname = smtp_hostname
        self.smtp_server = smtplib.SMTP(smtp_hostname)
        self.from_email = from_email

    def sendemail(self, to_emails, email_msg):
        try:
            self.smtp_server.sendmail(self.from_email, to_emails, email_msg)
        except smtplib.SMTPException as e:
            raise Exception("Failed to send email")


def main():
    cert_files = []
    sender_email = ""
    with open('config.json', 'r') as f:
        config = json.load(f)
        cert_files = config["files"]
        sender_email = config['sender_email']

    certs = []
    certs_html = ""
    for file_entry in cert_files:

        # http://stackoverflow.com/questions/8384737/python-extract-file-name-from-path-no-matter-what-the-os-path-format
        head, tail = os.path.split(file_entry['filename'])
        pwd = file_entry['password']
        for i in range(0, len(tail)):
            pwd = base64.b64decode(pwd)

        jks_checker = JksCertChecker(
            "./resources/" + file_entry['filename'],
            store_password=pwd)

        certs = jks_checker.get_trusted_certs()
        for private_key in file_entry['private_keys']:
            certs.extend(jks_checker.get_private_key_certs(private_key['alias'], private_key['password']))

        for cert in certs:
            print ("Certificate for " + file_entry['filename'])
            print (" * alias: " + str(cert.alias))
            print (" * issuer: " + str(cert.issuer))
            print (" * expiration: " + str(cert.expiration_date))
            print (" * Days left: " + str(cert.days_left))
            certs_html += """
<tr><td colspan="3" class="lightyellow">File: {}</td></tr>
<tr><td colspan="3" class="lightgreen">Issuer: {}</td></tr>
<tr row="3">
  <td>{}</td>
  <td>{}</td>
  <td>{}</td>
</tr>""".format(
                file_entry['filename'],
                cgi.escape(str(cert.issuer)).encode('ascii', 'xmlcharrefreplace'),
                cert.alias,
                str(cert.expiration_date),
                str(cert.days_left))

    #
    # Send email
    #
    email = MIMEMultipart('alternative')
    email['From'] = sender_email
    email['To'] = sender_email
    email['Subject'] = 'JKS Expiration status'

    # Create the body of the message (a plain-text and an HTML version).
    text = "Nothing here :)"
    html = """\
<html>
  <head>
    <style type="text/css">
      .border {{ border: 1px solid green; }}
      .lightyellow {{ background-color: #ffffaa; }}
      .lightgreen {{ background-color: #aaffcc; }}
      table {{ border-spacing: 0px; }}
      th, .row td {{ border: 1px double blue; }}
    </style>
  </head>
  <body>
    <table class="" border="1">
      <tr><th>Alias</th><th>Expiration Date</th><th>Days Before Expiration</th></tr>
      {}
    </table>
  </body>
</html>
""".format(certs_html)

    # Record the MIME types of both parts - text/plain and text/html.
    # Attach parts into message container.
    # According to RFC 2046, the last part of a multipart message, in this case
    # the HTML message, is best and preferred.
    email.attach(MIMEText(text, 'plain'))
    email.attach(MIMEText(html, 'html'))

    email_server = EmailServer('localhost', email['From'])
    email_server.sendemail(email['To'], email.as_string())

if __name__ == "__main__":
    main()
