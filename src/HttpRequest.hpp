#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.hpp"

// TODO change with parameter
#define BUFF_SIZE 3000

#define WHITESPACES " \t\v\f\r"

struct ReqLine
{
    std::string method;
    std::string target;
    std::string version;
};

struct ReqErr
{
    int code;
    std::string message;
};

class HttpRequest
{
private:
    std::map<std::string, std::string> _attrs;
    char _buff[BUFF_SIZE];
    ReqLine _req_line;
    ReqErr _err;

public:
    HttpRequest();
    ~HttpRequest();

    std::map<std::string, std::string> &get_attrs();
    char *get_buff();
    ReqLine &get_req_line();
    ReqErr &gett_err();

    void parse_req();

private:
    void _parse_req_line(std::string line);
    void _parse_attr_line(std::string line);
};

// TODO when not required anymore
std::ostream &operator<<(std::ostream &os, HttpRequest &std);