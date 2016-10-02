require 'net/http'
require 'net/https'
require 'uri'

## Check if the given URL is a valid site
## http://www.rubyinside.com/nethttp-cheat-sheet-2940.html
##
def httpcheck(url)
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
  return false;
  end

  # create a http object
  http = Net::HTTP.new(uri.host, uri.port); 
  http.open_timeout = 5;
  http.read_timeout = 5;

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
    return false
  end

  # make http call
  begin
    response = http.get('/');
    puts "* return type = " + response.code + " = " + response.class.to_s;
  rescue SocketError => e
    puts "SocketError(Invalid Host?) for '#{url}'";
  return false;
  end

end


##httpcheck("http://www.google.com");
httpcheck("http://www.example.com");
##httpcheck("http://www.example.com:80/boohoo");
httpcheck("https://www.yahoo.com");
httpcheck("https://ohohohohohohohohohohmyssl.com");
httpcheck("www.abc.com");
httpcheck("asdasdfasdfasdfsadfasdf");
httpcheck("asdasdfasdfasdfsadfasdf.com");


