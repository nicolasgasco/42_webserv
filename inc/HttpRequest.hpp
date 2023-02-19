#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "utils.hpp"
// TODO remove this after build is done
#include "dev_utils.hpp"

#define WHITESPACES " \t\v\f\r\n"

// TODO refactor this with something more scalable
// if some logic used elsewhere
#define LONGEST_METHOD "DELETE"

// TODO replace with computed value
#define LONGEST_URI 124

// TODO centralize this
#define HTTP_PROTOCOL "HTTP/1.1"

#define ACCEPT_HTML "text/html,application/xhtml+xml,application/xml"

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
    std::map<std::string, std::string> _params;
    std::string _buff;
    ReqLine _req_line;
    ReqErr _err;
    bool _req_complete;

public:
    HttpRequest();
    ~HttpRequest();

    // Getters
    std::map<std::string, std::string> const &get_attrs() const;
    std::string const &get_buff();
    ReqErr const &gett_err() const;
    std::map<std::string, std::string> const &get_params() const;
    ReqLine const &get_req_line() const;
    int get_content_length() const;
    bool get_req_complete() const;

    // Setters
    void set_buff(char const *buff);
    void set_req_complete(bool value);

    // Methods
    void parse_req();
    void output_status();

    // Computed properties
    bool has_error() const;
    bool has_query_params() const;
    bool is_html_req() const;

private:
    void _parse_attr_line(std::string const &line);
    void _parse_query_params(std::string &target);
    void _parse_req_line(std::string &line);
    void _parse_method(std::string &line);
    void _parse_target(std::string &line);
    void _parse_version(std::string &line);

    void _set_err(int const &code, std::string const &message);

    // Computed properties
    bool _is_method_supported() const;
};

// TODO remove when not required anymore
std::ostream &operator<<(std::ostream &os, HttpRequest &std);

std::ostream &operator<<(std::ostream &os, ReqErr &std);
