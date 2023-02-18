#pragma once

// TODO remove this after build is done
#include "dev_utils.hpp"
#include "HttpRequest.hpp"
#include "RouterService.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// TODO centralize this
#define HTTP_PROTOCOL "HTTP/1.1"

#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define NC "\033[0m"

struct StatusLine
{
    std::string version;
    int code;
    std::string reason;
};

class HttpResponse
{
private:
    std::string _buff;
    StatusLine _status_line;
    RouterService _router;

public:
    HttpResponse(HttpRequest const &req);
    ~HttpResponse();

    // Getters
    StatusLine const &get_status_line() const;
    std::string const &get_buff() const;

    // Setters
    void set_status_line(int const &code, std::string const &reason);

private:
    void _build_error_res(HttpRequest const &req);
    void _build_ok_res(HttpRequest const &req);

    void _replace_var_in_page(std::string &file, std::string const var, std::string const value) const;

    std::string _build_status_line() const;
    std::string _build_file(std::ifstream const &file);
};

// TODO remove when not required anymore
std::ostream &operator<<(std::ostream &os, HttpResponse &std);
