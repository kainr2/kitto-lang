

require 'net/http'
require 'net/https'
require 'uri'
##require 'oci8'


## Check if the given URL is a valid site
## http://www.rubyinside.com/nethttp-cheat-sheet-2940.html
##
## @param url  URL to check (not including query param)
## @return On fail, return 0.  On success, return HTTP status.
def checkUrl(url)
    zero_str = "0";
    puts "/////////////////////";
    puts "* calling #{url}";

    # parse URL.  The parser is pretty fragile, thus extra mod
    uri = nil;
    begin 
        uri = URI.parse(url);
        uri = URI.parse('http://' + url) if !uri.scheme;
        puts "* host=#{uri.host} port=#{uri.port}";
    rescue URI::Error => e
        puts "Invalid URL: '#{url}'";
        return zero_str;
    end

    # parse the path
    if (uri.path == nil || uri.path.empty?())
        uri.path = "/";
    end

    # create a http object
    http = Net::HTTP.new(uri.host, uri.port); 
    http.open_timeout = 10;  ## seconds
    http.read_timeout = 10;  ## seconds

    # set ssl
    if (uri.scheme == nil)
        http.use_ssl=(false);
    elsif (uri.scheme.downcase == "https")
        http.use_ssl=(true);
        http.verify_mode=(OpenSSL::SSL::VERIFY_NONE);
    elsif (uri.scheme.downcase == "http")
        http.use_ssl=(false);
    else
        puts "Invalid URL schema: '#{url}'";
        return zero_str;
    end

    # make http call
    http_code = zero_str;
    begin
        response = http.get(uri.path);
        http_code = response.code;
        puts "* return type = " + response.code + " = " + response.class.to_s;
    rescue SocketError => e
        puts "SocketError for '#{url}'";
        return zero_str;
    rescue Timeout::Error => e
        puts "Timeout::Error for '#{url}'";
        return zero_str;
    rescue Exception => msg
        puts "Generic Exception for '#{url}' -- #{msg}";
        return zero_str;
    end

    return http_code;
end


def checkDbi
    #log into DB and update LOA
    begin
        oci = SSODBI.new(@dbConnectionString)
        oci.updateLOA(@username, loaValue)
        oci.logoff()
    rescue Exception => msg
        @@log.error("ERROR: #{msg}")
    end
end    




def testCheckUrl()
    puts "HTTP status = " + checkUrl("http://www.example.com");
    puts "HTTP status = " + checkUrl("http://www.example.com:80");
    puts "HTTP status = " + checkUrl("http://www.google.com:443");
    puts "HTTP status = " + checkUrl("https://www.yahoo.com");
    puts "HTTP status = " + checkUrl("https://ohohohohohohohohohohmyssl.com");
    puts "HTTP status = " + checkUrl("www.abc.com");
    puts "HTTP status = " + checkUrl("asdasdfasdfasdfsadfasdf");
    puts "HTTP status = " + checkUrl("asdasdfasdfasdfsadfasdf.com");
    puts "HTTP status = " + checkUrl("http://ideas.noton.com/categories/2");
    puts "HTTP status = " + checkUrl("http://10.2.132.73:8080/user/me/my-views/view/Apollo/?");
end


testCheckUrl

