#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest const &req)
{
    if (req.has_error())
        this->_build_error_res(req);
    else
    {
        std::string method = req.get_req_line().method;
        if (method == "GET")
            this->_build_ok_res(req);
        else if (method == "POST")
        {
            // To be built
        }
        else if (method == "DELETE")
        {
            // To be built
        }
    }

    // TODO delete this when build is done
    std::cout << *this << std::endl
              << std::endl;
}

HttpResponse::~HttpResponse()
{
}

void HttpResponse::_build_error_res(HttpRequest const &req)
{
    int reqErrCode = req.gett_err().code;
    std::string reqErrMessage = req.gett_err().message;

    this->set_status_line(reqErrCode, reqErrMessage);
    this->_buff = this->_build_status_line();

    // TODO check if it's possible to always send HTML
    if (req.is_html_req())
    {

        std::ifstream file(build_path(PUBLIC_PATH, ERRORS_PATH, DEF_ERR_PAGE));

        std::string res_file = this->_build_file(file);
        this->_replace_var_in_page(res_file, "{{code}}", std::to_string(reqErrCode));
        this->_replace_var_in_page(res_file, "{{message}}", reqErrMessage);

        this->_buff += res_file;
    }
}

void HttpResponse::_build_ok_res(HttpRequest const &req)
{
    RouterService router;
    std::string file_path = router.get_file_path(req);

    std::ifstream file(file_path);

    if (file)
    {
        this->set_status_line(200, "OK");
        this->_buff = this->_build_file(file);
    }
    else
    {
        this->set_status_line(404, "Not Found");
        std::ifstream file_404(router.get_404_file_path());
        if (file_404)
            this->_buff = this->_build_file(file_404);
        else
            std::cerr << RED << "Error: missing 404.html file" << NC << std::endl;
    }

    // Pre-append status line
    this->_buff.insert(0, this->_build_status_line());
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