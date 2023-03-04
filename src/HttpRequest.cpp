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

    this->output_status();
}

// E.g. GET / HTTP/1.1
void HttpRequest::_parse_attr_line(std::string const &line)
{
    // A server MUST reject, with a response status code of 400 (Bad Request)
    // any received request message that contains whitespace between a header field name and colon
    bool is_space_before_colon = line.find(" :") != std::string::npos;
    if (is_space_before_colon)
        this->_set_err(400, "Bad Request");

    size_t colon_delim = line.find(":");
    std::string key = ltrim(line.substr(0, colon_delim));
    std::string value = trim(line.substr(colon_delim + 1));

    if (key.empty() || value.empty())
        this->_set_err(400, "Bad Request");

    this->_attrs.insert(std::pair<std::string, std::string>(key, value));

    // TODO Check Obsolete line folding
}

void HttpRequest::_parse_req_line(std::string &line)
{
    this->_parse_method(line);

    this->_parse_target(line);

    if (this->has_query_params())
        this->_parse_query_params(this->_req_line.target);

    this->_parse_version(line);
}

void HttpRequest::_parse_method(std::string &line)
{
    /* Recipients MAY instead parse on whitespace-delimited word boundaries and, aside from the CRLF terminator,
       treat any form of whitespace as the SP separator while ignoring preceding or trailing whitespace */
    size_t first_whitespace = line.find_first_of(WHITESPACES);
    std::string method = line.substr(0, first_whitespace);

    this->_req_line.method = trim(method);

    bool is_method_too_long = this->_req_line.method.length() > std::string(LONGEST_METHOD).length();
    if (is_method_too_long || !this->_is_method_supported()) // 501 - Method is too long or not supported
        this->_set_err(501, "Not Implemented");

    line = ltrim(line.substr(first_whitespace));
}

void HttpRequest::_parse_target(std::string &line)
{
    try
    {
        size_t first_whitespace = line.find_first_of(WHITESPACES);
        std::string target = line.substr(0, first_whitespace);

        this->_req_line.target = target;

        if (this->_req_line.target.empty())
            this->_set_err(400, "Bad Request");

        if (this->_req_line.target.length() > LONGEST_URI)
            this->_set_err(414, "URI Too Long");

        line = ltrim(line.substr(first_whitespace));
    }
    catch (const std::out_of_range &e)
    {
        return;
    }
}

void HttpRequest::_parse_version(std::string &line)
{
    /* Although the line terminator for the start-line and fields is the sequence CRLF,
       a recipient MAY recognize a single LF as a line terminator and ignore any preceding CR. */
    try
    {
        size_t first_newline = line.find("\n");

        std::string version = line.substr(0, first_newline);

        this->_req_line.version = trim(version);

        if (this->_req_line.version.empty() || this->_req_line.version != HTTP_PROTOCOL)
            this->_set_err(400, "Bad Request");
    }
    catch (const std::out_of_range &e)
    {
        return;
    }
}

void HttpRequest::_parse_query_params(std::string &target)
{
    size_t question_mark_sign_pos = target.find("?");
    std::string raw_params = target.substr(question_mark_sign_pos + 1);

    while (true)
    {
        size_t equal_sign_pos = raw_params.find("=");
        size_t ampersand_sign_pos = raw_params.find("&");

        std::string key = raw_params.substr(0, equal_sign_pos);
        std::string value = raw_params.substr(equal_sign_pos + 1, ampersand_sign_pos - equal_sign_pos - 1);

        this->_params.insert(std::pair<std::string, std::string>(key, value));

        raw_params = raw_params.substr(ampersand_sign_pos + 1);

        if (ampersand_sign_pos == std::string::npos)
            break;
    }

    target = target.substr(0, question_mark_sign_pos);
}

void HttpRequest::parse_post_req_body()
{
    std::vector<char> body = this->_body;

    std::vector<char>::iterator start;
    std::string search_pattern = "\r\n\r\n";
    for (std::vector<char>::iterator it = body.begin(); it != (body.end() - search_pattern.length()); ++it)
    {
        if (find_in_vec(search_pattern, it))
            start = it + 4;
    }
    body.erase(body.begin(), start);

    std::vector<char>::iterator end;
    search_pattern = "------WebKitFormBoundary";
    for (std::vector<char>::iterator it = body.begin(); it != (body.end() - search_pattern.length()); ++it)
    {
        if (find_in_vec(search_pattern, it))
        {
            end = it;
            break;
        }
    }
    body.erase(end, body.end());

    this->_body.clear();
    this->_body = body;
}

void HttpRequest::parse_post_req_file_name(std::string const &buff)
{
    std::string delim = "filename=\"";

    std::string file_name = buff.substr(buff.find(delim) + delim.length());
    file_name = file_name.substr(0, file_name.find("\""));

    this->_post_req_file_name = file_name;
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

std::vector<char> HttpRequest::get_body() const
{
    return this->_body;
}

std::string const &HttpRequest::get_buff() const
{
    return this->_buff;
}

std::string const &HttpRequest::get_post_req_file_name() const
{
    return this->_post_req_file_name;
}

std::map<std::string, std::string> const &HttpRequest::get_params() const
{
    return this->_params;
}

ReqLine const &HttpRequest::get_req_line() const
{
    return this->_req_line;
}

ReqErr const &HttpRequest::gett_err() const
{
    return this->_err;
}

std::map<std::string, std::string> const &HttpRequest::get_attrs() const
{
    return this->_attrs;
}

bool HttpRequest::has_body() const
{
    try
    {
        int content_length = std::stoi(this->_attrs.at("Content-Length"));
        std::string content_type = this->_attrs.at("Content-Type");

        if (content_length <= 0 || content_type.empty())
            return false;
        return true;
    }
    catch (const std::out_of_range &e)
    {
        return false;
    }
}

bool HttpRequest::has_error() const
{
    return this->_err.code != -1;
}

bool HttpRequest::has_query_params() const
{
    return this->_req_line.target.find("?") != std::string::npos;
}

bool HttpRequest::is_html_req() const
{
    bool is_root_target = this->_req_line.target == "/";

    // TODO investigate if Accept */* is required

    bool is_sec_fetch_dest_document;
    try
    {
        is_sec_fetch_dest_document = this->_attrs.at("Sec-Fetch-Dest") == "document";
    }
    catch (const std::out_of_range &oor)
    {
        is_sec_fetch_dest_document = false;
    }

    return (is_sec_fetch_dest_document || is_root_target);
}

bool HttpRequest::is_cgi_req() const
{
    return (this->_req_line.target.find("cgi_bin/") != std::string::npos);
}

bool HttpRequest::is_dir_req() const
{
    return (this->_req_line.target.back() == '/' && this->_req_line.target.length() != 1);
}

bool HttpRequest::_is_method_supported() const
{
    std::vector<std::string> supported_methods;
    supported_methods.push_back("GET");
    supported_methods.push_back("POST");
    supported_methods.push_back("DELETE");

    return (std::find(supported_methods.begin(), supported_methods.end(), this->_req_line.method) != supported_methods.end());
}

void HttpRequest::set_buff(char const *buff)
{
    this->_buff = buff;
}

void HttpRequest::set_body(std::vector<char> buff)
{
    if (this->_body.empty())
        this->_body = buff;
    else
        this->_body.insert(this->_body.end(), buff.begin(), buff.end());
}

void HttpRequest::_set_err(int const &code, std::string const &message)
{
    if (this->_err.code == -1)
    {
        this->_err.code = code;
        this->_err.message = message;
    }
}

void HttpRequest::reset()
{
    this->_attrs.clear();
    this->_params.clear();
    this->_body.clear();
    this->_buff.clear();
    this->_post_req_file_name.clear();

    this->_req_line.method.clear();
    this->_req_line.target.clear();
    this->_req_line.version.clear();

    this->_err.code = -1;
    this->_err.message.clear();
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

    for (std::map<std::string, std::string>::const_iterator it = std.get_attrs().begin(); it != std.get_attrs().end(); ++it)
        std::cout << it->first << ": " << YELLOW << "." << NC << it->second << YELLOW << "." << NC << std::endl;

    if (std.get_params().size())
    {
        std::cout << std::endl;
        std::cout << "QUERY PARAMS:" << std::endl;

        for (std::map<std::string, std::string>::const_iterator it = std.get_params().begin(); it != std.get_params().end(); ++it)
            std::cout << it->first << ": " << YELLOW << "." << NC << it->second << YELLOW << "." << NC << std::endl;
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, ReqErr &std)
{
    std::cerr << RED << std.code << ": " << std.message << NC;
    return os;
}