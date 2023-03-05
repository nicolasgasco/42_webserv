#pragma once

#include <iostream>
#include <string>
#include <vector>

std::string ltrim(std::string const &str);
std::string rtrim(std::string const &str);
std::string trim(std::string const &str);
bool str_isspace(std::string const &str);
bool str_ends_with(std::string const &value, std::string const &ending);

std::string build_path(std::string const &frag1, std::string const &frag2);
std::string build_path(std::string const &frag1, std::string const &frag2, std::string const &frag3);

std::string const get_gmt_time();
std::string const get_mime_type(std::string target);

void replace_var_in_page(std::string &file, std::string const var, std::string const value);

bool find_in_vec(std::string const &pattern, std::vector<char>::const_iterator it);