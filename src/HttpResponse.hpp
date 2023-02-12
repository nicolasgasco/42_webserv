#pragma once

// TODO remove this after build is done
#include "utils/dev_utils.hpp"
#include "HttpRequest.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// TODO centralize this
#define HTTP_PROTOCOL "HTTP/1.1"

// TODO centralize this
#define PUBLIC_PATH "public"

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

public:
    HttpResponse(HttpRequest &req);
    ~HttpResponse();

    StatusLine get_status_line();
    std::string get_buff();

private:
    void _build_status_line();
    void _build_message_body(std::string target);
};

// TODO remove when not required anymore
std::ostream &operator<<(std::ostream &os, HttpResponse &std);