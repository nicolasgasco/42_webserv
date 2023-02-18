#include "Configuration.hpp"
#include "Parser.hpp"

#include <fstream>
#include <iostream>
#include <vector>

Config::Config() {}

Config::~Config() {}

void	Config::check_config_file(char* config_file)
{
   	std::ifstream file;
    file.open(config_file);
	if (!file.is_open())
	{
		std::cout << "🔴  FAILURE Opening or reading config_file" << std::endl;
        exit(0);
	}
	std::cout << "🟢  SUCCESS Opening and reading config_file" << std::endl;
	
	std::ifstream input_data;
	std::string text;
	std::vector <std::string> text_vector;
	std::vector <std::string> words;

    input_data.open(config_file);
	
	while(getline(input_data,text))
	{
		text_vector.push_back(text);
	}

	words = parser_config_file(text_vector);
	check_words_config_file(text_vector);
}

