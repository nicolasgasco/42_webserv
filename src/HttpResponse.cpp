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
    this->set_status_line(req.gett_err().code, req.gett_err().message);
    this->_buff = this->_build_status_line();
}

void HttpResponse::_build_ok_res(HttpRequest const &req)
{
    RouterService router;
    std::string file_path = router.get_file_path(req);

    std::ifstream file(file_path);

    this->_buff = file ? this->_build_ok_page(file) : this->_build_404_page(req, router);
    // Pre-append status line
    this->_buff.insert(0, this->_build_status_line());
}

std::string HttpResponse::_build_status_line() const
{
    std::string status_line;
    status_line += this->_status_line.version + " ";
    status_line += std::to_string(this->_status_line.code) + " ";
    status_line += this->_status_line.reason + "\r\n";
    return status_line;
}

std::string HttpResponse::_build_ok_page(std::ifstream const &file)
{
    std::string message_body;

    std::ostringstream data_stream;

    data_stream << file.rdbuf();
    message_body += "\r\n" + data_stream.str();

    this->set_status_line(200, "OK");

    return message_body;
}

std::string HttpResponse::_build_404_page(HttpRequest const &req, RouterService const &router)
{
    this->set_status_line(400, "Not Found");

    std::string message_body_404_page;
    if (req.is_html_req())
    {
        std::ostringstream data_stream;

        std::ifstream file(router.get_404_file_path());
        if (file)
        {
            data_stream << file.rdbuf();
            message_body_404_page += "\r\n" + data_stream.str();
        }
        else
            std::cerr << RED << "Error: missing 404.html file" << NC << std::endl;
    }
    return message_body_404_page;
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