#include "HttpRequest.hpp"
#include <fstream> // std::ifstream

HttpRequest::HttpRequest()
{
    memset(this->_buff, 0, sizeof(this->_buff));

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
    if (this->has_error())
        return;

    // A recipient that receives whitespace between the start-line and the first header field
    // MUST either reject the message as invalid or consume each whitespace-preceded line
    // without further processing of it
    while (std::getline(buff_stream, line))
    {
        if (this->has_error())
            return;
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
    // A server MUST reject, with a response status code of 400 (Bad Request)
    // any received request message that contains whitespace between a header field name and colon
    if (line.find(" :") != std::string::npos)
        this->_set_err(400, "Bad Request");

    char *key_char_ptr = strtok(const_cast<char *>(line.c_str()), ":");
    char *value_char_ptr = strtok(NULL, "\n\r");

    if (!key_char_ptr || !value_char_ptr)
    {
        this->_set_err(400, "Bad Request");
        return;
    }

    std::string key_str(key_char_ptr);
    std::string value_str(value_char_ptr);
    this->_attrs.insert(std::pair<std::string, std::string>(ltrim(key_str), trim(value_str)));

    // TODO Check Obsolete line folding
}

void HttpRequest::_parse_req_line(std::string line)
{
    /* Recipients MAY instead parse on whitespace-delimited word boundaries and, aside from the CRLF terminator,
       treat any form of whitespace as the SP separator while ignoring preceding or trailing whitespace */
    std::string method_str = strtok(const_cast<char *>(line.c_str()), WHITESPACES);
    this->_req_line.method = trim(method_str);

    bool is_method_too_long = this->_req_line.method.length() > std::string(LONGEST_METHOD).length();
    if (is_method_too_long || !this->_is_method_supported()) // 501 - Method is too long or not supported
        this->_set_err(501, "Not Implemented");

    char *target_char_ptr = strtok(NULL, WHITESPACES);
    if (!target_char_ptr) // 400 - Malformed request line
    {
        this->_set_err(400, "Bad Request");
        return;
    }

    std::string target_str(target_char_ptr);
    this->_req_line.target = trim(target_str);

    if (this->_req_line.target.length() > LONGEST_URI) // 414 -Target too long
        this->_set_err(414, "URI Too Long");

    /* Although the line terminator for the start-line and fields is the sequence CRLF,
       a recipient MAY recognize a single LF as a line terminator and ignore any preceding CR. */
    char *version_char_ptr = strtok(NULL, "\n");
    if (!version_char_ptr) // 400 - Malformed request line
    {
        this->_set_err(400, "Bad Request");
        return;
    }

    std::string version_str(version_char_ptr);
    this->_req_line.version = trim(version_str);

    if (this->_req_line.version != HTTP_PROTOCOL) // 400 - Wrong HTTP protocol version
        this->_set_err(400, "Bad Request");

    // TODO
    // Recommended to support at least 8000 octets of request-line
}

void HttpRequest::output_status()
{
    // TODO delete this once build is over
    std::cout << *this << std::endl;

    if (this->_err.code != -1)
        std::cerr << this->_err << std::endl;
    else
        std::cout << YELLOW << "Valid request parsed..." << NC << std::endl;
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

bool HttpRequest::has_error()
{
    return this->_err.code != -1;
}

bool HttpRequest::is_html_req()
{
    return this->_attrs["Accept"].find(ACCEPT_HTML) != std::string::npos;
}

bool HttpRequest::_is_method_supported()
{
    std::vector<std::string> supported_methods;
    supported_methods.push_back("GET");
    supported_methods.push_back("POST");
    supported_methods.push_back("DELETE");

    return (std::find(supported_methods.begin(), supported_methods.end(), this->_req_line.method) != supported_methods.end());
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