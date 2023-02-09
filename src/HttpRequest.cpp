#include "HttpRequest.hpp"
#include <fstream> // std::ifstream

HttpRequest::HttpRequest()
{
    this->_err.code = 0;
}

HttpRequest::~HttpRequest()
{
}

void HttpRequest::parse_req()
{
    std::istringstream buff_stream(std::string(this->_buff));
    std::string line;

    // In the interest of robustness, a server that is expecting to receive and parse a request-line
    // SHOULD ignore at least one empty line (CRLF) received prior to the request-line.
    while (std::getline(buff_stream, line))
    {
        if (str_isspace(line))
            continue;
        else
        {
            std::cout << line << std::endl;
            this->_parse_req_line(line);
            break;
        }
    }

    // A recipient that receives whitespace between the start-line and the first header field
    // MUST either reject the message as invalid or consume each whitespace-preceded line
    // without further processing of it
    while (std::getline(buff_stream, line))
    {
        if (str_isspace(line))
        {
            if (this->_attrs.size())
                break;
            else
                continue;
        }
        else
            this->_parse_attr_line(line);
    }

    // TODO determine if message body is expected by reading header
    // If so, read as stream until amount of octets is depleted
    // Either Content-Length or Transfer-Encoding

    // TODO delete this once build is over
    std::cout << *this << std::endl;
}

// E.g. GET / HTTP/1.1
void HttpRequest::_parse_attr_line(std::string line)
{
    std::string key = strtok(const_cast<char *>(line.c_str()), ":");
    // A server MUST reject, with a response status code of 400 (Bad Request)
    // any received request message that contains whitespace between a header field name and colon
    if (key.find(' ') != std::string::npos)
    {
        this->_err.code = 400;
        this->_err.message = "Bad Request";
    }

    std::string value = strtok(NULL, "\n\r");

    this->_attrs.insert(std::pair<std::string, std::string>(ltrim(key), trim(value)));

    // TODO Check Obsolete line folding
}

// E.g. Cache-control: max-age=0
void HttpRequest::_parse_req_line(std::string line)
{
    // Recipients MAY instead parse on whitespace-delimited word boundaries and, aside from the CRLF terminator,
    // treat any form of whitespace as the SP separator while ignoring preceding or trailing whitespace
    std::string method_str = strtok(const_cast<char *>(line.c_str()), WHITESPACES);
    this->_req_line.method = trim(method_str);

    std::string target_str = strtok(NULL, WHITESPACES);
    this->_req_line.target = trim(target_str);

    // Although the line terminator for the start-line and fields is the sequence CRLF,
    // a recipient MAY recognize a single LF as a line terminator and ignore any preceding CR.
    std::string version_str = strtok(NULL, "\n");
    this->_req_line.version = trim(version_str);

    // TODO
    // Return 501 if Method is longer than any supported
    // Return 414 if URI is too long

    // TODO
    // Recommended to support at least 8000 octets of request-line
}

char *HttpRequest::get_buff()
{
    return this->_buff;
}

ReqLine &HttpRequest::get_req_line()
{
    return this->_req_line;
}

ReqErr &HttpRequest::gett_err()
{
    return this->_err;
}

std::map<std::string, std::string> &HttpRequest::get_attrs()
{
    return this->_attrs;
}

std::ostream &operator<<(std::ostream &os, HttpRequest &std)
{
    std::cout << std::endl;

    std::cout << "REQUEST LINE:" << std::endl;
    std::cout << "method: " << std.get_req_line().method << std::endl;
    std::cout << "request-target: " << std.get_req_line().target << std::endl;
    std::cout << "HTTP-version: " << std.get_req_line().version << std::endl;

    std::cout << std::endl;

    std::cout << "OPTIONS:" << std::endl;

    for (std::map<std::string, std::string>::iterator it = std.get_attrs().begin(); it != std.get_attrs().end(); ++it)
        std::cout << it->first << ": " << it->second << std::endl;

    return os;
}