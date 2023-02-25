#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest const &req, RouterService const &router) : _router(router), _req(req)
{
    this->_http = HttpService();
    this->_cgi = CgiService();

    if (this->_req.has_error())
        this->_build_error_res();
    else
    {
        std::string method = this->_req.get_req_line().method;
        if (method == "GET")
        {
            this->_build_get_res();
        }
        else if (method == "POST")
        {
            // To be built with CGI
        }
        else if (method == "DELETE")
            this->_build_delete_res();
    }

    // TODO delete this when build is done
    std::cout << *this << std::endl
              << std::endl;
}

HttpResponse::~HttpResponse()
{
}

void HttpResponse::_build_error_res()
{
    this->set_status_line(this->_req.gett_err().code, this->_req.gett_err().message);

    int content_len = 0;
    std::string res_body;

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
    this->_buff += this->_http.build_headers(content_len);
    this->_buff += res_body;
}

void HttpResponse::_build_get_res()
{

    int content_len = 0;
    std::string res_body;

    if (this->_req.is_dir_req())
    {
        this->set_status_line(200, "OK");

        res_body = this->_cgi.build_dir_content(this->_req.get_req_line().target);
        content_len = res_body.length() - CRLF_LEN;
    }
    else if (this->_req.is_cgi_req())
    {
        std::string file_path = this->_router.get_file_path(this->_req);
        std::ifstream file(file_path);

        if (file)
        {
            this->set_status_line(200, "OK");

            char *args[] = {const_cast<char *>(PYTHON3_PATH), const_cast<char *>(file_path.c_str()), NULL};
            std::string path = "PATH_INFO=" + std::string(GALLERY_STORAGE_PATH);
            char *envp[] = {const_cast<char *>(path.c_str()), NULL};
            res_body = this->_cgi.build_cgi_output(args, envp);

            content_len = res_body.length() - CRLF_LEN;
        }
        else
        {
            this->set_status_line(404, "Not Found");
            std::ifstream file_404(this->_router.get_404_file_path());
            res_body = this->_http.build_file(file_404);
            content_len = res_body.length() - CRLF_LEN;
        }
    }
    else
    {
        std::string file_path = this->_router.get_file_path(this->_req);
        std::ifstream file(file_path);

        if (file)
        {
            this->set_status_line(200, "OK");
            res_body = this->_http.build_file(file);
            content_len = res_body.length() - CRLF_LEN;
        }
        else
        {
            this->set_status_line(404, "Not Found");
            if (this->_req.is_html_req())
            {
                std::ifstream file_404(this->_router.get_404_file_path());
                res_body = this->_http.build_file(file_404);
                content_len = res_body.length() - CRLF_LEN;
            }
        }
    }

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len);
    this->_buff += res_body;
}

void HttpResponse::_build_delete_res()
{
    int content_len = 0;
    std::string res_body;

    this->set_status_line(200, "OK");

    char *args[] = {const_cast<char *>(PYTHON3_PATH), const_cast<char *>("./cgi_bin/delete_file.py"), NULL};
    std::string path = "PATH_INFO=" + std::string(PUBLIC_PATH) + std::string(this->_req.get_req_line().target);
    char *envp[] = {const_cast<char *>(path.c_str()), NULL};

    res_body = this->_cgi.build_cgi_output(args, envp);
    content_len = res_body.length() - CRLF_LEN;

    this->_buff = this->_http.build_status_line(this->_status_line.version, this->_status_line.code, this->_status_line.reason);
    this->_buff += this->_http.build_headers(content_len);
    this->_buff += res_body;
}

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

std::string HttpResponse::test_build_headers(int const &content_len) const
{
    return this->_http.build_headers(content_len);
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