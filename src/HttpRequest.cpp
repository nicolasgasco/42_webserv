#include "HttpRequest.hpp"
#include <fstream> // std::ifstream

HttpRequest::HttpRequest()
{
    this->_err.code = -1;
    this->_is_redirect = false;
}

HttpRequest::~HttpRequest()
{
}

/**
 * Parse request line and attributes.
 */
void HttpRequest::parse_req(Server const *server, Webserver *webserver)
{
    std::string body_str;
    body_str.reserve(this->_body.size());
    body_str.insert(body_str.begin(), this->_body.begin(), this->_body.end());

    std::istringstream buff_stream(body_str);
    std::string line;

    // In the interest of robustness, a server that is expecting to receive and parse a request-line
    // SHOULD ignore at least one empty line (CRLF) received prior to the request-line.
    while (std::getline(buff_stream, line))
    {
        if (str_isspace(line))
            continue;
        else
        {
            if (this->_parse_req_line(line, server) == 1)
                return;
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

    this->_validate_req_with_config(webserver);

    // TODO delete this once build is over
    this->output_status();
}

void HttpRequest::_validate_req_with_config(Webserver *webserver)
{
    std::string s = _req_line.target;
    std::string *path_com = NULL;
    std::string path;
    path_com = &path;
    int init = 0;
    int end = 0;
    while (end = s.find("/", init), end >= 0)
    {
        path = s.substr(init, end - init);
        if (path.length() > 0)
            path = "/" + path;
        init = end + 1;
    }
    std::cout << std::endl;

    try
    {
        std::string host = this->get_attrs().at("Host");
        this->port_browser = host.substr(host.find(":") + 1);
    }
    catch (std::out_of_range &e)
    {
        this->set_err(HTTP_400_CODE, HTTP_400_REASON);
        return;
    }

    for (std::vector<Server>::iterator it = webserver->_server.begin(); it != webserver->_server.end(); it++)
    {
        Server srv_data = *it;

        std::string path_to_find = this->get_req_line().target;
        std::cout << "🔵 PATH to find: " << path_to_find << std::endl;
        std::cout << "⭕️ PORT config_file: " << srv_data.get_port() << std::endl;
        std::cout << "⭕️ PORT browser:     " << port_browser << std::endl;
        if (srv_data.get_port() == this->port_browser)
        {
            std::vector<Location> locations = srv_data.get_location_blocks();
            for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
            {
                Location location = *it;
                std::cout << "🟠 location -> " << location.get_location() << std::endl;
                std::string new_path = *path_com;
                std::cout << "PATH     " << new_path << std::endl;
                std::cout << "LOCATION " << location.get_location() << std::endl;
                std::vector<std::string> methods = location.get_method();
                std::string alias = location.get_alias();

                std::cout << "🔵 ALIAS: "
                          << " -> " << alias << std::endl;

                if (path_to_find == alias)
                {
                    this->_req_line.target = location.get_location();
                    std::cout << "🟢 🟢  Alias exists in location: " << path_to_find << " & " << alias << "    Path changed to -> " << _req_line.target << std::endl;
                }

                if (this->_req_line.target == location.get_location())
                {
                    std::cout << "🟢 Path exists in location" << std::endl;
                    if (std::find(methods.begin(), methods.end(), this->_req_line.method) == methods.end())
                        this->set_err(HTTP_405_CODE, HTTP_405_REASON);
                }
            }
        }
    }
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
        this->set_err(HTTP_400_CODE, HTTP_400_REASON);

    size_t colon_delim = line.find(":");
    std::string key = ltrim(line.substr(0, colon_delim));
    std::string value = trim(line.substr(colon_delim + 1));

    if (key.empty() || value.empty())
        this->set_err(HTTP_400_CODE, HTTP_400_REASON);

    this->_attrs.insert(std::pair<std::string, std::string>(key, value));
}

/**
 * Parse request line, e.g. GET / HTTP/1.1.
 */
int HttpRequest::_parse_req_line(std::string &line, Server const *server)
{
    this->_parse_method(line);
    // If method is longer than the longest supported method
    if (this->has_error())
        return (1);

    this->_parse_target(line, server);

    if (this->has_query_params())
        this->_parse_query_params(this->_req_line.target);

    this->_parse_version(line);

    return (0);
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
    if (is_method_too_long || !this->_is_method_supported(this->_req_line.method))
        this->set_err(HTTP_501_CODE, HTTP_501_REASON);

    line = ltrim(line.substr(first_whitespace));
}

/**
 * Parse request line target, e.g. /index.hmtl.
 */
void HttpRequest::_parse_target(std::string &line, Server const *server)
{
    try
    {
        HttpService http;

        size_t first_whitespace = line.find_first_of(WHITESPACES);
        std::string target = line.substr(0, first_whitespace);

        this->_req_line.raw_target = target;
        this->_req_line.target = http.decode_whitespace(target);

        if (this->_req_line.target.empty())
            this->set_err(HTTP_400_CODE, HTTP_400_REASON);

        if (this->_req_line.target.length() > LONGEST_URI)
            this->set_err(HTTP_414_CODE, HTTP_414_REASON);

        line = ltrim(line.substr(first_whitespace));

        // Checking if it's a redirected asset
        if (this->_is_target_redirection(this->_req_line.target, const_cast<Server *>(server)))
            this->_is_redirect = true;
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
            this->set_err(HTTP_400_CODE, HTTP_400_REASON);
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
 * Parse boundary used by browser when sending post request.
 * @returns Parsed boundary.
 */
std::string const HttpRequest::_parse_post_req_boundary() const
{
    std::string body_str = std::string(this->_body.data());

    std::string delim = POST_BODY_BOUNDARY;
    body_str = body_str.substr(body_str.find(delim) + delim.length());

    std::string boundary = body_str.substr(0, body_str.find("\r\n"));

    return boundary;
}

std::vector<char> const &HttpRequest::get_body() const
{
    return this->_body;
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

bool HttpRequest::get_is_redirection() const
{
    return this->_is_redirect;
}

/**
 * Checks if request has a body.
 * It must have either Content-Length or Transfer-Encoding headers
 */
bool HttpRequest::has_body() const
{
    int content_length = -1;
    std::string transfer_encoding;

    try
    {
        content_length = std::stoi(this->_attrs.at(CONTENT_LENGTH));
        if (content_length < 0)
            return false;
    }
    catch (const std::out_of_range &e)
    {
    }

    try
    {
        transfer_encoding = this->_attrs.at(TRANSFER_ENCODING);
        if (transfer_encoding.empty())
            return false;
    }
    catch (const std::out_of_range &e)
    {
    }

    if (content_length == -1 && transfer_encoding.empty())
        return false;
    return true;
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
    return (this->_req_line.target.find(CGI_BIN_PATH) != std::string::npos);
}

/**
 * Checks if request is for a directory.
 * If it doesn't have a file extension, it is a directory.
 */
bool HttpRequest::is_dir_req() const
{
    return this->_req_line.target.find(".") == std::string::npos;
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

bool HttpRequest::_is_method_supported(std::string method) const
{
    if (method == "GET" || method == "POST" || method == "DELETE" || method == "HEAD")
        return true;
    return false;
}
bool HttpRequest::_is_target_redirection(std::string const &target, Server *server) const
{
    // Check if the current route has redirect directive
    std::vector<Location> locations = server->get_location_blocks();
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        std::string index_page = it->get_index();
        if (index_page.size() == 0)
            continue;
        else
        {
            std::string config_target = build_path(it->get_location(), index_page);
            if (target == config_target && !it->get_redirect().empty())
                return true;
        }
    }
    return false;
}

void HttpRequest::set_body(std::vector<char> &buff)
{
    this->_body.insert(this->_body.end(), buff.begin(), buff.end());
}

void HttpRequest::set_err(int const &code, std::string const &message)
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

    this->_req_line.method.clear();
    this->_req_line.target.clear();
    this->_req_line.raw_target.clear();
    this->_req_line.version.clear();

    this->_err.code = -1;
    this->_err.message.clear();

    this->_is_redirect = false;
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
    {
        std::cout << it->first << ": " << YELLOW << "." << NC << it->second << YELLOW << "." << NC << std::endl;
        if (it->first == "Host")
        {
            std::string str = it->second;
            std::size_t pos = str.find(":");
            std.port_browser = str.substr(pos + 1);
        }
    }

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
