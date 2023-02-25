#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest const &req, RouterService const &router) : _router(router), _req(req)
{
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
        {
            // To be built with CGI
        }
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

        std::string err_page = this->_build_file(file);
        this->_replace_var_in_page(err_page, "{{code}}", std::to_string(this->_req.gett_err().code));
        this->_replace_var_in_page(err_page, "{{message}}", this->_req.gett_err().message);

        res_body = err_page;
        content_len = err_page.length() - CRLF_LEN;
    }

    this->_buff = this->_build_status_line();
    this->_buff += this->_build_headers(content_len);
    this->_buff += res_body;
}

void HttpResponse::_build_get_res()
{
    CgiService cgi;

    int content_len = 0;
    std::string res_body;

    if (this->_req.is_dir_req())
    {
        this->set_status_line(200, "OK");

        res_body = cgi.build_dir_content(this->_req.get_req_line().target);
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
            res_body = cgi.build_cgi_output(args, envp);

            content_len = res_body.length() - CRLF_LEN;
        }
        else
        {
            this->set_status_line(404, "Not Found");
            std::ifstream file_404(this->_router.get_404_file_path());
            res_body = this->_build_file(file_404);
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
            res_body = this->_build_file(file);
            content_len = res_body.length() - CRLF_LEN;
        }
        else
        {
            this->set_status_line(404, "Not Found");
            if (this->_req.is_html_req())
            {
                std::ifstream file_404(this->_router.get_404_file_path());
                res_body = this->_build_file(file_404);
                content_len = res_body.length() - CRLF_LEN;
            }
        }
    }

    this->_buff = this->_build_status_line();
    this->_buff += this->_build_headers(content_len);
    this->_buff += res_body;
}

std::string HttpResponse::_build_file(std::ifstream const &file)
{
    std::string message_body;

    std::ostringstream data_stream;

    data_stream << file.rdbuf();

    message_body += "\r\n";
    message_body += data_stream.str();

    return message_body;
}

std::string HttpResponse::_build_status_line() const
{
    std::string status_line;
    status_line += this->_status_line.version + " " + std::to_string(this->_status_line.code) + " " + this->_status_line.reason;
    status_line += "\r\n";
    return status_line;
}

std::string HttpResponse::_build_headers(int const &content_len) const
{
    std::string headers;

    std::string date = "Date: " + get_gmt_time() + "\r\n";
    std::string server = "Server: " + std::string(DEFAULT_SERVER_NAME) + "/1.0" + "\r\n";
    std::string content_length = "Content-Length: " + std::to_string(content_len) + "\r\n";
    // std::string content_type = this->_get_content_type(this->_req.get_req_line().target);

    headers += date;
    headers += server;
    headers += content_length;
    // headers += content_type;

    return headers;
}

void HttpResponse::_replace_var_in_page(std::string &file, std::string const var, std::string const value) const
{
    while (true)
    {
        size_t posCode = file.find(var);
        if (posCode == std::string::npos)
            return;

        file.erase(posCode, var.length());
        file.insert(posCode, value);
    }
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
    return this->_build_headers(content_len);
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