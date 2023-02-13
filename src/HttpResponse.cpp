#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest &req)
{
    _status_line.version = HTTP_PROTOCOL;

    if (req.has_error())
    {
        _status_line.code = req.gett_err().code;
        _status_line.reason = req.gett_err().message;

        this->_buff = this->_build_status_line();
    }
    else
    {
        this->_buff = this->_build_message_body(req);
        // Pre-append status line
        this->_buff.insert(0, this->_build_status_line());
    }

    // TODO delete this when build is done
    std::cout << *this << std::endl;
    std::cout << std::endl;
}

HttpResponse::~HttpResponse()
{
}

StatusLine HttpResponse::get_status_line()
{
    return this->_status_line;
}

std::string HttpResponse::get_buff()
{
    return this->_buff;
}

std::string HttpResponse::_build_status_line()
{
    std::string status_line;
    status_line += this->_status_line.version + " ";
    status_line += std::to_string(this->_status_line.code) + " ";
    status_line += this->_status_line.reason + "\r\n";
    return status_line;
}

std::string HttpResponse::_build_message_body(HttpRequest &req)
{
    std::string message_body;

    RouterService router(req);
    std::string file_path = router.get_file_path();

    std::ifstream file(file_path);
    std::ostringstream data_stream;

    if (file)
    {
        data_stream << file.rdbuf();

        message_body += "\r\n" + data_stream.str();

        this->_status_line.code = 200;
        this->_status_line.reason = "OK";

        return message_body;
    }

    this->_err.code = 404;
    this->_err.message = "Not Found";
    // TODO delete this when build is over
    std::cerr << std::endl
              << RED << this->_err << NC << std::endl;

    this->_status_line.code = this->_err.code;
    this->_status_line.reason = this->_err.message;

    // 404 - Not Found - Only for page request
    if (req.is_html_req())
        message_body += this->_build_404_page();

    return message_body;
}

std::string HttpResponse::_build_404_page()
{
    std::ostringstream data_stream;

    std::string message_body_404_page;

    std::string err_404_path = build_path(PUBLIC_PATH, ERRORS_PATH, "404.html");

    std::ifstream err_file(err_404_path);
    if (err_file)
    {
        data_stream << err_file.rdbuf();
        message_body_404_page += "\r\n" + data_stream.str();

        this->_status_line.code = 404;
        this->_status_line.reason = "Not Found";
    }

    return message_body_404_page;
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