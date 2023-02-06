#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{
}

HttpRequest::~HttpRequest()
{
}

char *HttpRequest::get_buff()
{
    return this->_buff;
}

std::vector<std::string> &HttpRequest::get_req_line()
{
    return this->_req_line;
}

std::map<std::string, std::string> &HttpRequest::get_attrs()
{
    return this->_attrs;
}

void HttpRequest::parse_req()
{
    std::istringstream buff_stream(std::string(this->_buff));

    std::string line;
    while (std::getline(buff_stream, line))
    {
        if (line.find(":") != std::string::npos)
            this->_parse_attr_line(line);
        else
            this->_parse_req_line(line);
    }
    this->_parse_attr_line(line);
    std::cout << *this << std::endl;
}

// E.g. GET / HTTP/1.1
void HttpRequest::_parse_attr_line(std::string line)
{
    std::string key = strtok(const_cast<char *>(line.c_str()), ": ");
    std::string value = strtok(NULL, "\n\r");

    std::pair<std::string, std::string> key_value_pair(key, ltrim(value));
    this->_attrs.insert(key_value_pair);
}

// E.g. Cache-control: max-age=0
void HttpRequest::_parse_req_line(std::string line)
{
    // Method
    std::string method = strtok(const_cast<char *>(line.c_str()), " ");
    this->_req_line.push_back(method);

    // Request target
    std::string target = strtok(NULL, " ");
    this->_req_line.push_back(target);

    // HTTP version
    std::string version_str = strtok(NULL, "\n\r");
    this->_req_line.push_back(version_str);
}

std::ostream &operator<<(std::ostream &os, HttpRequest &std)
{
    std::cout << std::endl;

    std::cout << "REQUEST LINE:" << std::endl;
    std::cout << "method: |" << std.get_req_line()[0] << "|" << std::endl;
    std::cout << "request-target: |" << std.get_req_line()[1] << "|" << std::endl;
    std::cout << "HTTP-version: |" << std.get_req_line()[2] << "|" << std::endl;

    std::cout << std::endl;

    std::cout << "OPTIONS:" << std::endl;

    for (std::map<std::string, std::string>::iterator it = std.get_attrs().begin(); it != std.get_attrs().end(); ++it)
        std::cout << it->first << ": |" << it->second << "|" << std::endl;

    return os;
}