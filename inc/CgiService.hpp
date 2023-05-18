#pragma once

#include "macros.hpp"

#include "RouterService.hpp"
#include "HttpRequest.hpp"
#include "Server.hpp"

#include <iostream>
#include <unistd.h>

class CgiService
{
public:
    CgiService();
    ~CgiService();

    std::string const build_cgi_output(char *const *args, char *const *envp) const;
    std::string const build_cgi_output(char *const *args, char *const *envp, const std::vector<char> *req_body) const;
    std::vector<std::string> build_envp(std::string path, Server const *server, HttpRequest const &req) const;

    std::string const get_cgi_executable(std::string const &path) const;

private:
    std::string const _build_500_error_page() const;
};