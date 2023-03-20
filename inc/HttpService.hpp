#pragma once

#include "macros.hpp"
#include "utils.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

class HttpService
{
public:
    HttpService();
    ~HttpService();

    std::string const build_file(std::ifstream const &file) const;
    std::string const build_headers(int const &content_len, std::string const &server_name, std::string const &content_type) const;
    std::string const build_status_line(std::string const &version, int const &code, std::string const &reason) const;

    std::string const decode_whitespace(std::string const &target) const;
};
