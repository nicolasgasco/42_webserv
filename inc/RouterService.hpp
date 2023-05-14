#pragma once

#include "HttpRequest.hpp"
#include "macros.hpp"
#include "utils.hpp"

class RouterService
{
public:
    RouterService();
    ~RouterService();

    std::string const get_file_path(HttpRequest const &req, Server *server) const;
    std::string const get_404_file_path() const;
    std::string const get_def_err_file_path() const;

private:
};
