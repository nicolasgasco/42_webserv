#pragma once

#include <iostream>
#include <string>

std::string ltrim(std::string &str);
std::string rtrim(std::string &str);
std::string trim(std::string &str);
bool str_isspace(std::string &str);

std::string build_path(std::string frag1, std::string frag2);
std::string build_path(std::string frag1, std::string frag2, std::string frag3);