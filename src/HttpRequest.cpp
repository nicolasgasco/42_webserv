#include "HttpRequest.hpp"
#include <fstream> // std::ifstream

HttpRequest::HttpRequest()
{
    this->_err.code = -1;
}

HttpRequest::~HttpRequest()
{
}

void HttpRequest::parse_req()
{
    // TODO remove this when build is over
    std::cout << std::endl
              << "RAW REQUEST:" << std::endl;
    cout_explicit_whitespaces(std::string(this->_buff));

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
            this->_parse_req_line(line);
            break;
        }
    }
    if (this->_err.code != -1)
        return;

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
}

// E.g. GET / HTTP/1.1
void HttpRequest::_parse_attr_line(std::string line)
{
    std::string key = strtok(const_cast<char *>(line.c_str()), ":");
    // A server MUST reject, with a response status code of 400 (Bad Request)
    // any received request message that contains whitespace between a header field name and colon
    if (key.find(' ') != std::string::npos)
        this->_set_err(400, "Bad Request");

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

    // A server that receives a method longer than any that it implements
    // SHOULD respond with a 501 (Not Implemented) status code
    if (this->_req_line.method.length() > std::string(LONGEST_METHOD).length())
        this->_set_err(501, "Not Implemented");

    char *target_char_ptr = strtok(NULL, WHITESPACES);
    if (target_char_ptr)
    {
        std::string target_str(target_char_ptr);
        this->_req_line.target = trim(target_str);
    }
    else
        this->_set_err(400, "Bad Request");

    // A server that receives a request-target longer than any URI it wishes to parse
    // MUST respond with a 414 (URI Too Long) status code
    if (this->_req_line.target.length() > LONGEST_URI)
        this->_set_err(414, "URI Too Long");

    // Although the line terminator for the start-line and fields is the sequence CRLF,
    // a recipient MAY recognize a single LF as a line terminator and ignore any preceding CR.
    char *version_char_ptr = strtok(NULL, "\n");
    if (version_char_ptr)
    {
        std::string version_str(version_char_ptr);
        this->_req_line.version = trim(version_str);
    }
    else
        this->_set_err(400, "Bad Request");

    // TODO
    // Recommended to support at least 8000 octets of request-line
}

void HttpRequest::output_status()
{
    if (this->_err.code != -1)
        std::cerr << this->_err << std::endl;
    else
    {
        std::cout << YELLOW << "Valid request parsed..." << NC << std::endl;

        // TODO delete this once build is over
        std::cout << *this << std::endl;
    }
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

void HttpRequest::_set_err(int code, std::string message)
{
    this->_err.code = code;
    this->_err.message = message;
}

std::ostream &operator<<(std::ostream &os, HttpRequest &std)
{
    std::cout << std::endl;

    std::cout << "REQUEST LINE:" << std::endl;
    std::cout << "method: " << YELLOW << "." << NC << std.get_req_line().method << YELLOW << "." << NC << std::endl;
    std::cout << "request-target: " << YELLOW << "." << NC << std.get_req_line().target << YELLOW << "." << NC << std::endl;
    std::cout << "HTTP-version: " << YELLOW << "." << NC << std.get_req_line().version << YELLOW << "." << NC << std::endl;

    std::cout << std::endl;

    std::cout << "OPTIONS:" << std::endl;

    for (std::map<std::string, std::string>::iterator it = std.get_attrs().begin(); it != std.get_attrs().end(); ++it)
        std::cout << it->first << ": " << YELLOW << "." << NC << it->second << YELLOW << "." << NC << std::endl;

    return os;
}

std::ostream &operator<<(std::ostream &os, ReqErr &std)
{
    std::cerr << RED << std.code << ": " << std.message << NC;
    return os;
}