#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.hpp"

// TODO change with parameter
#define BUFF_SIZE 3000

class HttpRequest
{
private:
    std::map<std::string, std::string> _attrs;
    char _buff[BUFF_SIZE];
    std::vector<std::string> _req_line;

public:
    HttpRequest();
    ~HttpRequest();

    std::map<std::string, std::string> &get_attrs();
    char *get_buff();
    std::vector<std::string> &get_req_line();

    void parse_req();

private:
    void _parse_req_line(std::string line);
    void _parse_attr_line(std::string line);
};

// TODO when not required anymore
std::ostream &operator<<(std::ostream &os, HttpRequest &std);