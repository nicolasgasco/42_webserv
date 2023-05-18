#pragma once

#include "CgiService.hpp"
#include "HttpRequest.hpp"
#include "HttpService.hpp"
#include "macros.hpp"
#include "RouterService.hpp"
#include "Server.hpp"

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
    RouterService const &_router;
    HttpService _http;
    CgiService _cgi;
    Server *_server;

    std::string _buff;
    HttpRequest _req;
    StatusLine _status_line;

public:
    HttpResponse(RouterService const &router);
    ~HttpResponse();

    // Getters
    std::string const &get_buff() const;
    StatusLine const &get_status_line() const;

    // Setters
    void set_buff(std::string const &buff);
    void set_status_line(int const &code, std::string const &reason);

    // Methods
    void build_response(HttpRequest req, HttpService const &http, CgiService const &cgi, Server *server);
    void reset();

private:
    void _build_delete_res();
    void _build_get_res(std::string method);
    void _build_error_res();
    void _build_post_res();
    std::string const _build_cgi_res(std::string const &path, const std::vector<char> *req_body);

    void _replace_var_in_page(std::string &file, std::string const var, std::string const value) const;

    std::string const _get_content_type(std::string target) const;

public:
    // For testing purposes only
    std::string test_build_headers(int const &content_len) const;
};

std::ostream &operator<<(std::ostream &os, HttpResponse &std);
