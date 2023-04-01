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
    this->_buff += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
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
        std::vector<std::string> envp_v = this->_cgi.build_envp(path, this->_server, this->_req);
        char *envp[CGI_MAX_ENV_VARS];
        for (size_t i = 0; i < envp_v.size(); i++)
            envp[i] = const_cast<char *>(envp_v[i].c_str());
        envp[envp_v.size()] = NULL;

        res_body = this->_cgi.build_cgi_output(args, envp, nullptr);

        content_len = res_body.length() - CRLF_LEN;
    }
    // If a CGI script is required
    else if (this->_req.is_cgi_req())
        this->_build_cgi_res(GALLERY_STORAGE_PATH, res_body, content_len, nullptr);
    // It's a normal asset
    else
    {
        std::string file_path = this->_router.get_file_path(this->_req);
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
                if (this->_req.is_html_req())
                {
                //  std::ifstream file_404(this->_router.get_404_file_path());
				//	std::cout << "🏆 " << _router.get_404_file_path() << std::endl;
                    
					std::string err_page_path = _server->get_error_page();
					err_page_path.erase(0,1);
				//	std::cout << "🏆 " << err_page_path << std::endl;

					std::ifstream path_404(err_page_path.c_str());	
						
				//	res_body = this->_http.build_file(file_404);
					res_body = this->_http.build_file(path_404);
                    content_len = res_body.length() - CRLF_LEN;
                }
            }
        }
    }

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
    // TODO change with real redirection logic
    if (this->_req.get_is_redirection())
        this->_buff += "Location: " + std::string("/redirect/redirect_page.html") + "\r\n";
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

    std::string file_path = this->_req.get_post_req_file_name();
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
        std::vector<char> const body = this->_req.get_body();
        this->_build_cgi_res(file_path, res_body, content_len, &body);

        this->set_status_line(HTTP_200_CODE, HTTP_200_REASON);
        std::ifstream res_file(GALLERY_SUCCESS_TEMPLATE_PATH);
        res_body = this->_http.build_file(res_file);
        content_len = res_body.length() - CRLF_LEN;
    }

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
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
        std::ifstream f(target.c_str());

        // If file you want to DELETE exists
        if (f.good())
            this->set_status_line(HTTP_200_CODE, HTTP_200_REASON);
        else
            this->set_status_line(HTTP_404_CODE, HTTP_404_REASON);

        std::string cgi_script_path = build_path(PUBLIC_PATH, CGI_BIN_PATH, "delete_file.py");
        char *args[] = {const_cast<char *>(PYTHON3_PATH), const_cast<char *>(cgi_script_path.c_str()), NULL};

        // Environment variables for CGI script
        std::vector<std::string> envp_v = this->_cgi.build_envp(target, this->_server, this->_req);
        char *envp[CGI_MAX_ENV_VARS];
        for (size_t i = 0; i < envp_v.size(); i++)
            envp[i] = const_cast<char *>(envp_v[i].c_str());
        envp[envp_v.size()] = NULL;

        res_body = this->_cgi.build_cgi_output(args, envp, nullptr);
        content_len = res_body.length() - CRLF_LEN;
    }

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len, this->_server->get_server_name(), this->_get_content_type(this->_req.get_req_line().target));
    this->_buff += res_body;
}

/**
 * Execute CGI script and return its output.
 */
void HttpResponse::_build_cgi_res(std::string const &path, std::string &res_body, int &content_len, const std::vector<char> *req_body)
{
    std::string file_path = this->_router.get_file_path(this->_req);
    std::ifstream file(file_path);

    if (file)
    {
        this->set_status_line(HTTP_200_CODE, HTTP_200_REASON);

        std::string executable = this->_cgi.get_cgi_executable(file_path);
        char *args[] = {const_cast<char *>(executable.c_str()), const_cast<char *>(file_path.c_str()), NULL};

        // Environment variables for CGI script
        std::vector<std::string> envp_v = this->_cgi.build_envp(path, this->_server, this->_req);
        char *envp[CGI_MAX_ENV_VARS];
        for (size_t i = 0; i < envp_v.size(); i++)
            envp[i] = const_cast<char *>(envp_v[i].c_str());
        envp[envp_v.size()] = NULL;
        res_body = this->_cgi.build_cgi_output(args, envp, req_body);
        content_len = res_body.length() - CRLF_LEN;
    }
    else
    {
        this->set_status_line(HTTP_404_CODE, HTTP_404_REASON);
    //  std::ifstream file_404(this->_router.get_404_file_path());
    //  res_body = this->_http.build_file(file_404);
                    
		std::string err_page_path = _server->get_error_page();
		err_page_path.erase(0,1);
		std::ifstream path_404(err_page_path.c_str());	
		res_body = this->_http.build_file(path_404);

        content_len = res_body.length() - CRLF_LEN;
    }
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
