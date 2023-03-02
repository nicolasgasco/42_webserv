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

    std::string const build_cgi_output(char *const *args, char *const *envp) const;
};