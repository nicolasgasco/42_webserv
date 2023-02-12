#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.hpp"
// TODO remove this after build is done
#include "utils/dev_utils.hpp"

// TODO change with parameter
#define BUFF_SIZE 3000

#define WHITESPACES " \t\v\f\r\n"

// TODO refactor this with something more scalable
// if some logic used elsewhere
#define LONGEST_METHOD "DELETE"

// TODO replace with computed value
#define LONGEST_URI 124

// TODO centralize this
#define HTTP_PROTOCOL "HTTP/1.1"

#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define NC "\033[0m"

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

    // Getters
    std::map<std::string, std::string> &get_attrs();
    char *get_buff();
    ReqLine &get_req_line();
    ReqErr &gett_err();

    // Methods
    void parse_req();
    void output_status();

    // Computed properties
    bool has_error();

private:
    void _parse_req_line(std::string line);
    void _parse_attr_line(std::string line);

    void _set_err(int code, std::string message);

    // Computed properties
    bool _is_method_supported();
};

// TODO remove when not required anymore
std::ostream &operator<<(std::ostream &os, HttpRequest &std);

std::ostream &operator<<(std::ostream &os, ReqErr &std);