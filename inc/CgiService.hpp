#pragma once

#include "macros.hpp"
#include "RouterService.hpp"

#include <iostream>
#include <unistd.h>

class CgiService
{
public:
    CgiService();
    ~CgiService();

    std::string const build_dir_content(std::string const &target) const;
};