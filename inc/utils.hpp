#pragma once

#include <iostream>
#include <string>

std::string ltrim(std::string const &str);
std::string rtrim(std::string const &str);
std::string trim(std::string const &str);
bool str_isspace(std::string const &str);
bool str_ends_with(std::string const &value, std::string const &ending);

std::string build_path(std::string const &frag1, std::string const &frag2);
std::string build_path(std::string const &frag1, std::string const &frag2, std::string const &frag3);