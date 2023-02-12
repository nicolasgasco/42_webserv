#include "HttpResponse.hpp"

HttpResponse::HttpResponse(int &code, std::string &reason)
{
    _status_line.version = HTTP_PROTOCOL;
    _status_line.code = code;
    _status_line.reason = reason;

    this->_build_status_line();
    this->_build_message_body();

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

void HttpResponse::_build_message_body()
{
    // TODO add logic to fetch file dynamically
    std::ifstream file("public/index.html");
    std::string buffer_str;

    if (file)
    {
        std::ostringstream data_stream;
        data_stream << file.rdbuf(); // reading data
        this->_buff += "\r\n";
        this->_buff += data_stream.str();
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

    std::cout << "BODY:" << std::endl;
    std::cout << std.get_buff() << std::endl;

    return os;
}