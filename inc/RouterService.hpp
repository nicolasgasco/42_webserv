#pragma once

#include "HttpRequest.hpp"

#define YELLOW "\033[0;33m"
#define NC "\033[0m"

#define PUBLIC_PATH "public"
#define ERRORS_PATH "error"

class RouterService
{
private:
    HttpRequest &_req;

public:
    RouterService(HttpRequest &req);
    ~RouterService();

    std::string get_file_path();

private:
};