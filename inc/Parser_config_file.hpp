#pragma once

#include <vector>
#include <iostream>

std::vector <std::string> parser_config_file(std::vector <std::string> text_vector);
void check_words_config_file(std::vector<std::string> text_vector);

typedef std::vector<std::string>::iterator vector_iterator;
int check_server_block(std::vector<std::string> server_block);
int location_block_size(vector_iterator it, vector_iterator end);

std::vector <std::string>	parser_vec(const std::string &str);
std::string					parser_str(const std::string &str);
int 						parser_num(const std::string &str);
void 						parser_fail(const std::string &str);
