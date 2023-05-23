#include "HttpResponse.hpp"

HttpResponse::HttpResponse(RouterService const &router) : _router(router)
{
}

HttpResponse::~HttpResponse()
{
}

/**
 * Takes a request and returns a response.
 */
void HttpResponse::build_response(HttpRequest req, HttpService const &http, CgiService const &cgi, Server *server)
{
    this->_req = req;
    this->_http = http;
    this->_cgi = cgi;
    this->_server = server;

    if (this->_req.has_error())
        this->_build_error_res();
    else
    {
        std::string method = this->_req.get_req_line().method;
        if (method == "GET")
            this->_build_get_res("GET");
        else if (method == "HEAD")
            this->_build_get_res("HEAD");
        else if (method == "POST")
            this->_build_post_res();
        else if (method == "DELETE")
            this->_build_delete_res();
    }

    std::cout << *this << std::endl
              << std::endl;
}

/**
 * Build response when there is an error.
 */
void HttpResponse::_build_error_res()
{
    this->set_status_line(this->_req.gett_err().code, this->_req.gett_err().message);

    std::ifstream file(this->_router.get_def_err_file_path());
    std::string err_page = this->_http.build_file(file);
    replace_var_in_page(err_page, "{{code}}", std::to_string(this->_req.gett_err().code));
    replace_var_in_page(err_page, "{{message}}", this->_req.gett_err().message);

    std::string res_body = err_page;
    int content_len = err_page.length();

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
    this->_buff += "\r\n";
    this->_buff += res_body;
}

/**
 * Build response when it is GET request.
 */
void HttpResponse::_build_get_res(std::string method)
{
    int content_len = 0;
    std::string res_body;

    std::string file_path = this->_router.get_file_path(this->_req, this->_server);

    // If a folder is required and autoindex is off, use CGI script to output folder content
    if (this->_req.is_dir_req() && this->_server->get_autoindex() == false)
    {
        std::ifstream file(file_path);

        if (!file)
        {
            this->set_status_line(HTTP_404_CODE, HTTP_404_REASON);

            std::string err_page_path = trim_trailing_leading_slash(_server->get_error_page());
            std::ifstream path_404(err_page_path.c_str());
            std::string err_page_body = this->_http.build_file(path_404);
            int content_len = err_page_body.size();

            this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
            this->_buff += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
            this->_buff += "\r\n";
            this->_buff += err_page_body;
            return;
        }
        else
        {
            this->set_status_line(0, "N/A");

            std::string cgi_script_path = build_path(CGI_BIN_PATH, OUTPUT_DIR_SCRIPT);
            char *args[] = {const_cast<char *>(PYTHON3_PATH), const_cast<char *>(cgi_script_path.c_str()), NULL};

            // Environment variables for CGI script
            std::string path = build_path(PUBLIC_PATH, this->_req.get_req_line().target);
            std::vector<std::string> envp_v = this->_cgi.build_envp(path, this->_server, this->_req);
            char *envp[CGI_MAX_ENV_VARS];
            for (size_t i = 0; i < envp_v.size(); i++)
                envp[i] = const_cast<char *>(envp_v[i].c_str());
            envp[envp_v.size()] = NULL;

            this->_buff = this->_cgi.build_cgi_output(args, envp);
            return;
        }
    }
    // If a CGI script is required
    else if (this->_req.is_cgi_req())
    {
        std::string gallery_path = this->_server->get_storage().size() ? trim_trailing_leading_slash(this->_server->get_storage()) : GALLERY_STORAGE_PATH;
        this->_buff = this->_build_cgi_res(gallery_path, nullptr, nullptr);
        return;
    }
    // It's an asset request
    else
    {
        std::ifstream file(file_path);

        if (this->_req.get_is_redirection())
            this->set_status_line(HTTP_301_CODE, HTTP_301_REASON);
        else
        {
            if (file)
            {
                this->set_status_line(HTTP_200_CODE, HTTP_200_REASON);
                res_body = this->_http.build_file(file);
                content_len = res_body.length() - CRLF_LEN;
            }
            else
            {
                this->set_status_line(HTTP_404_CODE, HTTP_404_REASON);
                std::string err_page_path = _server->get_error_page();
                err_page_path.erase(0, 1);
                std::ifstream path_404(err_page_path.c_str());
                res_body = this->_http.build_file(path_404);
                content_len = res_body.length() - CRLF_LEN;
            }
        }
    }

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
    // Find redirect value and send it with Location header
    if (this->_req.get_is_redirection())
    {
        std::vector<Location> locations = this->_server->get_location_blocks();
        for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
        {
            std::string index_page = it->get_index();
            if (index_page.size() > 0)
            {
                if (this->_req.get_req_line().target == build_path(it->get_location(), index_page))
                {
                    this->_buff += "Location: " + build_path(it->get_location(), it->get_redirect()) + "\r\n";
                    break;
                }
            }
        }
    }

    if (method == "GET")
        this->_buff += res_body;
    else if (method == "HEAD")
        this->_buff += "\r\n";
}

/**
 * Build response when it is POST request.
 */
void HttpResponse::_build_post_res()
{
    std::string file_path = GALLERY_PAGE_SCRIPT;

    std::vector<char> req_body = this->_req.get_body();

    std::string req_body_str = std::string(req_body.data());
    size_t body_size = req_body_str.substr(req_body_str.find("\r\n\r\n") + 4).size();

    std::string target = this->_router.get_file_path(this->_req, this->_server);
    bool is_cgi_target = target.find(CGI_BIN_PATH) != std::string::npos;
    bool has_file_extension = target.substr(1).find(".") != std::string::npos;
    bool is_content_type_form_data;
    try
    {
        is_content_type_form_data = this->_req.get_attrs().at(CONTENT_TYPE).find("multipart/form-data") != std::string::npos;
    }
    catch (std::out_of_range &e)
    {
        is_content_type_form_data = false;
    }

    if ((this->_req.has_body() && body_size > 0) && (is_cgi_target && has_file_extension) && is_content_type_form_data)
    {
        std::string gallery_path = this->_server->get_storage().size() ? trim_trailing_leading_slash(this->_server->get_storage()) : GALLERY_STORAGE_PATH;
        std::string user_defined_env = "GALLERY_PATH=" + gallery_path;
        this->_buff = this->_build_cgi_res(file_path, &user_defined_env, &req_body);
    }
    else
    {
        this->set_status_line(HTTP_400_CODE, HTTP_400_REASON);

        std::ifstream file(this->_router.get_def_err_file_path());
        std::string err_page = this->_http.build_file(file);
        replace_var_in_page(err_page, "{{code}}", std::to_string(HTTP_400_CODE));
        replace_var_in_page(err_page, "{{message}}", HTTP_400_REASON);

        std::string res_body = err_page;
        int content_len = err_page.length();

        this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
        this->_buff += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
        this->_buff += "\r\n";
        this->_buff += res_body;
    }
}

/**
 * Build response when it is DELETE request.
 */
void HttpResponse::_build_delete_res()
{
    int content_len = 0;
    std::string res_body;
    std::string target = PUBLIC_PATH + this->_req.get_req_line().target;

    std::string storage_path = this->_server->get_storage();
    // Remove leading point if exists
    storage_path = storage_path.at(0) == '.' ? storage_path.substr(1) : storage_path;
    std::string gallery_path = this->_server->get_storage().size() ? trim_trailing_leading_slash(storage_path) : GALLERY_STORAGE_PATH;

    bool is_allowed_path = target.find(gallery_path) != std::string::npos;
    if (!is_allowed_path)
    {
        this->set_status_line(HTTP_403_CODE, HTTP_403_REASON);

        std::ifstream err_file(this->_router.get_def_err_file_path());
        std::string err_page_body = this->_http.build_file(err_file);
        replace_var_in_page(err_page_body, "{{code}}", std::to_string(HTTP_403_CODE));
        replace_var_in_page(err_page_body, "{{message}}", HTTP_403_REASON);

        content_len = err_page_body.length();

        res_body = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
        res_body += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
        res_body += "\r\n";
        res_body += err_page_body;
    }
    else if (target.find(".") == std::string::npos)
    {
        this->set_status_line(HTTP_400_CODE, HTTP_400_REASON);

        std::ifstream err_file(this->_router.get_def_err_file_path());
        std::string err_page_body = this->_http.build_file(err_file);
        replace_var_in_page(err_page_body, "{{code}}", std::to_string(HTTP_400_CODE));
        replace_var_in_page(err_page_body, "{{message}}", HTTP_400_REASON);

        content_len = err_page_body.length();

        res_body = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
        res_body += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
        res_body += "\r\n";
        res_body += err_page_body;
    }
    else
    {
        this->set_status_line(0, "N/A");

        std::string cgi_script_path = build_path(CGI_BIN_PATH, "delete_file.py");
        char *args[] = {const_cast<char *>(PYTHON3_PATH), const_cast<char *>(cgi_script_path.c_str()), NULL};

        // Environment variables for CGI script
        std::vector<std::string> envp_v = this->_cgi.build_envp(target, this->_server, this->_req);
        char *envp[CGI_MAX_ENV_VARS];
        for (size_t i = 0; i < envp_v.size(); i++)
            envp[i] = const_cast<char *>(envp_v[i].c_str());
        envp[envp_v.size()] = NULL;

        res_body = this->_cgi.build_cgi_output(args, envp);
    }

    this->_buff = res_body;
}

/**
 * Execute CGI script and return its output.
 */
std::string const HttpResponse::_build_cgi_res(std::string const &path, std::string *user_defined_env, const std::vector<char> *req_body)
{
    std::string file_path = this->_router.get_file_path(this->_req, this->_server);
    std::ifstream file(file_path);

    // If file exists and it's not a folder
    if (file && file_path.substr(1).find(".") != std::string::npos)
    {
        this->set_status_line(0, "N/A");

        std::string executable = this->_cgi.get_cgi_executable(file_path);
        char *args[] = {const_cast<char *>(executable.c_str()), const_cast<char *>(file_path.c_str()), NULL};

        // Environment variables for CGI script
        std::vector<std::string> envp_v = this->_cgi.build_envp(path, this->_server, this->_req);
        if (user_defined_env)
            envp_v.push_back(*user_defined_env);
        char *envp[CGI_MAX_ENV_VARS];
        for (size_t i = 0; i < envp_v.size(); i++)
            envp[i] = const_cast<char *>(envp_v[i].c_str());
        envp[envp_v.size()] = NULL;

        return req_body ? this->_cgi.build_cgi_output(args, envp, req_body) : this->_cgi.build_cgi_output(args, envp);
    }

    this->set_status_line(HTTP_400_CODE, HTTP_400_REASON);

    std::ifstream err_file(this->_router.get_def_err_file_path());
    std::string err_page_body = this->_http.build_file(err_file);
    replace_var_in_page(err_page_body, "{{code}}", std::to_string(HTTP_400_CODE));
    replace_var_in_page(err_page_body, "{{message}}", HTTP_400_REASON);

    int content_len = err_page_body.length();

    std::string res_body = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    res_body += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
    res_body += "\r\n";
    res_body += err_page_body;

    return res_body;
}

/**
 * Get content type for a specific target URI.
 */
std::string const HttpResponse::_get_content_type(std::string target) const
{
    std::string content_type;

    std::string mime_type = get_mime_type(target);
    if (mime_type.length())
        content_type = "Content-Type: " + mime_type + "\r\n";
    else if (this->_req.is_html_req())
        content_type = "Content-Type: text/html\r\n";

    return content_type;
}

StatusLine const &HttpResponse::get_status_line() const
{
    return this->_status_line;
}

std::string const &HttpResponse::get_buff() const
{
    return this->_buff;
}

void HttpResponse::set_status_line(int const &code, std::string const &reason)
{
    this->_status_line.version = std::string(HTTP_PROTOCOL);
    this->_status_line.code = int(code);
    this->_status_line.reason = std::string(reason);
}

void HttpResponse::set_buff(std::string const &buff)
{
    this->_buff = buff;
}

void HttpResponse::reset()
{
    this->_buff.clear();

    this->_status_line.version.clear();
    this->_status_line.code = -1;
    this->_status_line.reason.clear();
}

// For testing only
std::string HttpResponse::test_build_headers(int const &content_len) const
{
    return this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
}

std::ostream &operator<<(std::ostream &os, HttpResponse &std)
{
    std::cout << std::endl;

    std::cout << "STATUS LINE:" << std::endl;
    std::cout << "version: " << YELLOW << "." << NC << std.get_status_line().version << YELLOW << "." << NC << std::endl;
    std::cout << "status-code: " << YELLOW << "." << NC << std.get_status_line().code << YELLOW << "." << NC << std::endl;
    std::cout << "reason-phrase: " << YELLOW << "." << NC << std.get_status_line().reason << YELLOW << "." << NC << std::endl;

    std::cout << std::endl;

    return os;
}
