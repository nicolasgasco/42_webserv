#pragma once

#include "HttpRequest.hpp"
#include "utils.hpp"

#define YELLOW "\033[0;33m"
#define NC "\033[0m"

#define PUBLIC_PATH "public"
#define ERRORS_PATH "error"

class RouterService
{
public:
    RouterService();
    ~RouterService();

    std::string get_file_path(HttpRequest const &req);

private:
};