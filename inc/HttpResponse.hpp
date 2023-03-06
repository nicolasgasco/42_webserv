#pragma once

#include "CgiService.hpp"
#include "dev_utils.hpp" // TODO remove this after build is done
#include "HttpRequest.hpp"
#include "HttpService.hpp"
#include "macros.hpp"
#include "RouterService.hpp"
#include "Webserver.hpp"

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
    Webserver _server;

    std::string _buff;
    HttpRequest _req;
    StatusLine _status_line;

public:
    HttpResponse(RouterService const &router, class Webserver *webserver);
    ~HttpResponse();

    // Getters
    std::string const &get_buff() const;
    StatusLine const &get_status_line() const;

    // Setters
    void set_buff(std::string const &buff);
    void set_status_line(int const &code, std::string const &reason);

    // Methods
    void build_response(HttpRequest req, HttpService const &http, CgiService const &cgi, class Webserver *webserver);
    void reset();

private:
    void _build_delete_res(class Webserver *webserver);
    void _build_get_res(class Webserver *webserver);
    void _build_error_res(class Webserver *webserver);
    void _build_post_res(class Webserver *webserver);

    void _replace_var_in_page(std::string &file, std::string const var, std::string const value) const;

    std::string const _get_content_type(std::string target) const;

public:
    // For testing purposes only
    std::string test_build_headers(int const &content_len, class Webserver *webserver) const;
};

// TODO remove when not required anymore
std::ostream &operator<<(std::ostream &os, HttpResponse &std);
