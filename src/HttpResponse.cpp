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
void HttpResponse::build_response(HttpRequest req, HttpService const &http, CgiService const &cgi, std::string const &server_name)
{
    this->_req = req;
    this->_http = http;
    this->_cgi = cgi;
    this->_server_name = server_name;

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

    // TODO delete this when build is done
    std::cout << *this << std::endl
              << std::endl;
}

/**
 * Build response when there is an error.
 */
void HttpResponse::_build_error_res()
{
    this->set_status_line(this->_req.gett_err().code, this->_req.gett_err().message);

    int content_len = 0;
    std::string res_body;

    // If the request was for an HTML page, sends error page
    if (this->_req.is_html_req())
    {
        std::ifstream file(this->_router.get_def_err_file_path());

        std::string err_page = this->_http.build_file(file);
        replace_var_in_page(err_page, "{{code}}", std::to_string(this->_req.gett_err().code));
        replace_var_in_page(err_page, "{{message}}", this->_req.gett_err().message);

        res_body = err_page;
        content_len = err_page.length() - CRLF_LEN;
    }

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len, this->_server_name);
    // Required for empty body POST request
    this->_buff += res_body.empty() ? "\r\n" : res_body;
}

/**
 * Build response when it is GET request.
 */
void HttpResponse::_build_get_res(std::string method)
{

    int content_len = 0;
    std::string res_body;

    // If a folder is required, use CGI script to output folder content
    if (this->_req.is_dir_req())
    {
        this->set_status_line(HTTP_200_CODE, HTTP_200_REASON);

        std::string cgi_script_path = build_path(PUBLIC_PATH, CGI_BIN_PATH, "output_dir_content.py");
        char *args[] = {const_cast<char *>(PYTHON3_PATH), const_cast<char *>(cgi_script_path.c_str()), NULL};

        // Environment variables for CGI script
        std::string path = build_path(PUBLIC_PATH, this->_req.get_req_line().target);
        std::vector<std::string> envp_v = this->_cgi.build_envp(path, this->_req);
        char *envp[CGI_MAX_ENV_VARS];
        for (size_t i = 0; i < envp_v.size(); i++)
            envp[i] = const_cast<char *>(envp_v[i].c_str());
        envp[envp_v.size()] = NULL;

        res_body = this->_cgi.build_cgi_output(args, envp);

        content_len = res_body.length() - CRLF_LEN;
    }
    // If a CGI script is required
    else if (this->_req.is_cgi_req())
    {
        std::string file_path = this->_router.get_file_path(this->_req);
        std::ifstream file(file_path);

        if (file)
        {
            this->set_status_line(HTTP_200_CODE, HTTP_200_REASON);

            std::string executable = this->_cgi.get_cgi_executable(file_path);
            char *args[] = {const_cast<char *>(executable.c_str()), const_cast<char *>(file_path.c_str()), NULL};

            // Environment variables for CGI script
            std::vector<std::string> envp_v = this->_cgi.build_envp(GALLERY_STORAGE_PATH, this->_req);
            char *envp[CGI_MAX_ENV_VARS];
            for (size_t i = 0; i < envp_v.size(); i++)
                envp[i] = const_cast<char *>(envp_v[i].c_str());
            envp[envp_v.size()] = NULL;
            res_body = this->_cgi.build_cgi_output(args, envp);
            content_len = res_body.length() - CRLF_LEN;
        }
        else
        {
            this->set_status_line(HTTP_404_CODE, HTTP_404_REASON);
            std::ifstream file_404(this->_router.get_404_file_path());
            res_body = this->_http.build_file(file_404);
            content_len = res_body.length() - CRLF_LEN;
        }
    }
    // If it's another type of asset
    else
    {
        std::string file_path = this->_router.get_file_path(this->_req);
        std::ifstream file(file_path);

        if (file)
        {
            this->set_status_line(HTTP_200_CODE, HTTP_200_REASON);
            res_body = this->_http.build_file(file);
            content_len = res_body.length() - CRLF_LEN;
        }
        else
        {
            this->set_status_line(HTTP_404_CODE, HTTP_404_REASON);
            if (this->_req.is_html_req())
            {
                std::ifstream file_404(this->_router.get_404_file_path());
                res_body = this->_http.build_file(file_404);
                content_len = res_body.length() - CRLF_LEN;
            }
        }
    }

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len, this->_server_name);
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
    int content_len = 0;
    std::string res_body;

    std::string file_path = build_path(GALLERY_STORAGE_PATH, this->_req.get_post_req_file_name());
    std::ifstream f(file_path.c_str());

    // If file you want to POST exists already
    if (f.good())
    {
        this->set_status_line(HTTP_409_CODE, HTTP_409_REASON);

        std::ifstream file(this->_router.get_def_err_file_path());

        std::string err_page = this->_http.build_file(file);
        replace_var_in_page(err_page, "{{code}}", std::to_string(HTTP_409_CODE));
        replace_var_in_page(err_page, "{{message}}", HTTP_409_REASON);

        res_body = err_page;
        content_len = err_page.length() - CRLF_LEN;
    }
    else
    {
        std::stringstream file(file_path);
        std::ofstream img(file.str().c_str(), std::ios::binary);
        img.write(this->_req.get_body().data(), this->_req.get_body().size());
        img.close();

        this->set_status_line(HTTP_200_CODE, HTTP_200_REASON);
        std::ifstream res_file(GALLERY_SUCCESS_TEMPLATE_PATH);
        res_body = this->_http.build_file(res_file);
        content_len = res_body.length() - CRLF_LEN;
    }

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len, this->_server_name);
    this->_buff += res_body;
}

/**
 * Build response when it is DELETE request.
 */
void HttpResponse::_build_delete_res()
{
    int content_len = 0;
    std::string res_body;
    std::string target = PUBLIC_PATH + this->_req.get_req_line().target;

    // TODO add real logic to check if DELETE is allowed on route
    bool is_allowed_path = target.find(GALLERY_STORAGE_PATH) != std::string::npos;
    if (!is_allowed_path)
        this->set_status_line(HTTP_401_CODE, HTTP_401_REASON);
    else
    {
        std::string cgi_script_path = build_path(PUBLIC_PATH, CGI_BIN_PATH, "delete_file.py");
        char *args[] = {const_cast<char *>(PYTHON3_PATH), const_cast<char *>(cgi_script_path.c_str()), NULL};

        // Environment variables for CGI script
        std::vector<std::string> envp_v = this->_cgi.build_envp(target, this->_req);
        char *envp[CGI_MAX_ENV_VARS];
        for (size_t i = 0; i < envp_v.size(); i++)
            envp[i] = const_cast<char *>(envp_v[i].c_str());
        envp[envp_v.size()] = NULL;

        std::string cgi_output = this->_cgi.build_cgi_output(args, envp);

        // CGI script returns 404 error code when file to delete wasn't found
        if (cgi_output.find(std::to_string(HTTP_404_CODE)) != std::string::npos)
            this->set_status_line(HTTP_404_CODE, HTTP_404_REASON);
        else
            this->set_status_line(HTTP_200_CODE, HTTP_200_REASON);

        res_body = cgi_output;
        content_len = res_body.length() - CRLF_LEN;
    }

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len, this->_server_name);
    this->_buff += res_body;
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
    return this->_http.build_headers(content_len, this->_server_name);
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
