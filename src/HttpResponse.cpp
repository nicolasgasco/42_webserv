#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest &req)
{
    _status_line.version = HTTP_PROTOCOL;
    // TODO compute this instead of mocking
    _status_line.code = req.has_error() ? req.gett_err().code : 200;
    // TODO compute this instead of mocking
    _status_line.reason = req.has_error() ? req.gett_err().message : "OK";

    this->_build_status_line();
    this->_build_message_body(req.get_req_line().target);

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

void HttpResponse::_build_status_line()
{
    this->_buff += this->_status_line.version + " ";
    this->_buff += std::to_string(this->_status_line.code) + " ";
    this->_buff += this->_status_line.reason + "\r\n";
}

void HttpResponse::_build_message_body(std::string target)
{
    std::string file_path;
    if (target == "/")
    {
        file_path += PUBLIC_PATH;
        file_path += "/index.html";
    }
    else
    {
        // TODO add logic to fetch file dynamically
        file_path = PUBLIC_PATH + target;
    }

    std::ifstream file(file_path);
    std::string buffer_str;

    if (file)
    {
        std::ostringstream data_stream;
        data_stream << file.rdbuf(); // reading data
        this->_buff += "\r\n";
        this->_buff += data_stream.str();
    }
    else
    {
        // TODO file doesn't exist
        std::cerr << RED << "File does not exist..." << NC << std::endl;
    }
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