#include "HttpRequest.hpp"
#include <fstream> // std::ifstream

HttpRequest::HttpRequest()
{
    this->_err.code = -1;
}

HttpRequest::~HttpRequest()
{
}

/**
 * Parse request line and attributes.
 */
void HttpRequest::parse_req()
{
    // TODO remove this when build is over
    std::cout << std::endl
              << "RAW REQUEST:" << std::endl;
    cout_explicit_whitespaces(std::string(this->_body.data()));

    std::istringstream buff_stream(std::string(this->_body.data()));
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
    // TODO delete this once build is over
    this->output_status();
}

/**
 * Parse attribute, e.g. Accept-Encoding: gzip, deflate.
 */
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
}

/**
 * Parse request line, e.g. GET / HTTP/1.1.
 */
void HttpRequest::_parse_req_line(std::string &line)
{
    this->_parse_method(line);

    this->_parse_target(line);

    if (this->has_query_params())
        this->_parse_query_params(this->_req_line.target);

    this->_parse_version(line);
}

/**
 * Parse request line method, e.g. GET.
 */
void HttpRequest::_parse_method(std::string &line)
{
    /* Recipients MAY instead parse on whitespace-delimited word boundaries and, aside from the CRLF terminator,
       treat any form of whitespace as the SP separator while ignoring preceding or trailing whitespace */
    size_t first_whitespace = line.find_first_of(WHITESPACES);
    std::string method = line.substr(0, first_whitespace);

    this->_req_line.method = trim(method);

    bool is_method_too_long = this->_req_line.method.length() > std::string(LONGEST_METHOD).length();
    if (is_method_too_long || !this->_is_method_supported()) // 501 - Method is too long or not supported
        this->_set_err(HTTP_501_CODE, HTTP_501_REASON);

    line = ltrim(line.substr(first_whitespace));
}

/**
 * Parse request line target, e.g. /index.hmtl.
 */
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

/**
 * Parse request line version, e.g. HTTP/1.1.
 */
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

/**
 * Parse query params in target, e.g. /index.hmtl&user=bar.
 */
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

/**
 * Parse body of request, delimited by browser boundary.
 */
void HttpRequest::parse_post_req_body()
{
    std::vector<char> body = this->_body;

    std::vector<char>::const_iterator start;
    std::string search_pattern = "\r\n\r\n";
    for (std::vector<char>::const_iterator it = body.begin(); it != (body.end() - search_pattern.length()); ++it)
    {
        if (find_in_vec(search_pattern, it))
            start = it + 4;
    }
    body.erase(body.begin(), start);

    std::vector<char>::const_iterator end;
    search_pattern = CHROME_BODY_BOUNDARY;
    for (std::vector<char>::const_iterator it = body.begin(); it != (body.end() - search_pattern.length()); ++it)
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

    // If an empty POST request was sent
    if (this->_body.empty())
        this->_set_err(400, "Bad Request");
}

/**
 * Parse file name of binary data sent in POST request.
 */
void HttpRequest::parse_post_req_file_name(std::vector<char> const &body)
{
    std::string file_name;

    std::string start_delim = "filename=\"";
    std::vector<char>::const_iterator start;
    for (std::vector<char>::const_iterator it = body.begin(); it != body.end(); ++it)
    {
        if (find_in_vec(start_delim, it))
        {
            start = it + start_delim.length();
            break;
        }
    }

    std::string end_delim = "\"";
    for (std::vector<char>::const_iterator it = start; it != body.end(); ++it)
    {
        if (find_in_vec(end_delim, it))
            break;
        file_name.push_back(*it);
    }

    this->_post_req_file_name = file_name;
}

std::vector<char> const &HttpRequest::get_body() const
{
    return this->_body;
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

/**
 * Checks if request has a body.
 * It must have either Content-Length or Content-Type headers
 */
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

bool HttpRequest::is_cgi_req() const
{
    return (this->_req_line.target.find("cgi_bin/") != std::string::npos);
}

/**
 * Checks if request is for a directory.
 * Root folder is not included.
 */
bool HttpRequest::is_dir_req() const
{
    // Ends with / but it's not root folder request
    return (this->_req_line.target.back() == '/' && this->_req_line.target.length() != 1);
}

/**
 * Checks if request is for an HTML document.
 * If Sec-Fetch-Dest header is set to document or if it is root folder.
 */
bool HttpRequest::is_html_req() const
{
    // Check if Sec-Fetch-Dest is set to document
    bool is_sec_fetch_dest_document;
    try
    {
        is_sec_fetch_dest_document = this->_attrs.at("Sec-Fetch-Dest") == "document";
    }
    catch (const std::out_of_range &oor)
    {
        is_sec_fetch_dest_document = false;
    }

    bool is_root_target = this->_req_line.target == "/";

    return (is_sec_fetch_dest_document || is_root_target);
}

bool HttpRequest::_is_method_supported() const
{
    std::vector<std::string> supported_methods;
    supported_methods.push_back("GET");
    supported_methods.push_back("POST");
    supported_methods.push_back("DELETE");

    return (std::find(supported_methods.begin(), supported_methods.end(), this->_req_line.method) != supported_methods.end());
}

void HttpRequest::set_body(std::vector<char> &buff)
{
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

/**
 * Resets req object state.
 * Requests are long-lived in main function.
 */
void HttpRequest::reset()
{
    this->_attrs.clear();
    this->_params.clear();
    this->_body.clear();
    this->_post_req_file_name.clear();

    this->_req_line.method.clear();
    this->_req_line.target.clear();
    this->_req_line.version.clear();

    this->_err.code = -1;
    this->_err.message.clear();
}

void HttpRequest::output_status()
{
    std::cout << *this << std::endl;

    if (this->_err.code != -1)
        std::cerr << this->_err << std::endl;
    else
        std::cout << YELLOW << "Valid request parsed..." << NC << std::endl;
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