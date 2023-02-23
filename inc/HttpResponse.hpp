#pragma once

#include "dev_utils.hpp" // TODO remove this after build is done
#include "HttpRequest.hpp"
#include "macros.hpp"
#include "RouterService.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
    RouterService const &_router;
    HttpRequest const &_req;

public:
    HttpResponse(HttpRequest const &req, RouterService const &router);
    ~HttpResponse();

    // Getters
    StatusLine const &get_status_line() const;
    std::string const &get_buff() const;

    // Setters
    void set_status_line(int const &code, std::string const &reason);

private:
    void _build_error_res();
    void _build_get_res();

    std::string _build_status_line() const;
    std::string _build_headers(int const &content_len) const;
    std::string _build_file(std::ifstream const &file);

    void _replace_var_in_page(std::string &file, std::string const var, std::string const value) const;
    std::string const _get_content_type(std::string target) const;

public:
    // For testing purposes only
    std::string test_build_headers(int const &content_len) const;
};

// TODO remove when not required anymore
std::ostream &operator<<(std::ostream &os, HttpResponse &std);
