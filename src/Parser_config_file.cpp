#include "Parser_config_file.hpp"

#include <sstream>

std::vector <std::string> split_line(std::vector <std::string> text_vector)
{
	std::vector <std::string> words;
	for (std::vector <std::string>::iterator it = text_vector.begin(); it != text_vector.end(); ++it)
	{
		std::string line = *it;
		std::stringstream input(line);
		std::string str_tmp;		
		while (std::getline(input, str_tmp, ' '))
		{
			if (str_tmp == "")
				continue;
			words.push_back(str_tmp);
        }
    }
    return (words);
}

std::vector <std::string> remove_tabs(std::vector <std::string> words)
{
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
	{
		std::string input = *it;
		it->erase(std::remove(it->begin(), it->end(), '\t'), it->end());
    }
    return (words);
}

void check_file_curly_brackets(std::vector <std::string> words)
{
    unsigned long i = 0;
    int count = 0;

    while ( i < words.size())
    {
        if (words[i] == "{")
        	++count;
        else if (words[i] == "}")
        	--count;
        i++;
    }
    if (count != 0)
    {
		std::string errorMessage = std::string("🔴  FAILURE Wrong config_file formated (curly_brackets)");
		throw std::runtime_error(errorMessage);
    }
	std::cout << "🟢  SUCCESS Right config_file formated (curly_brackets)" << std::endl;
}

std::vector <std::string> parser_config_file(std::vector <std::string> text_vector)
{
    std::vector<std::string> words;

    words = split_line(text_vector);
    if (words.size() == 0)
    {
		std::string errorMessage = std::string("🔴  FAILURE Wrong config_file formated (empty file)");
		throw std::runtime_error(errorMessage);
    }
	std::cout << "🟢  SUCCESS Right config_file formated (not empty file)" << std::endl;
	words = remove_tabs(words);
    check_file_curly_brackets(words);
	return (words);
}


int check_directives_server(std::string word)
{
	std::string errorMessage;
	errorMessage = "🔴  FAILURE: [";
    errorMessage += word;
    errorMessage += "] directives in config_file is not a valid syntax";
    if (word != "server" && 
		word != "port" && 
		word != "host" && 
		word != "server_name" && 
		word != "root" && 
		word != "error_page" && 
		word != "cgi_file_ext" && 
		word != "autoindex" && 
		word != "max_body_size" && 
		word != "location" && 
		word != "alias" && 
		word != "accepted_methods" && 
		word != "index" && 
		word != "{" && 
		word != "}") 
	{
		throw std::runtime_error(errorMessage);
	}
	return (0);
}

void check_words_config_file(std::vector<std::string> text_vector)
{
	std::vector <std::string> tmp;
	std::vector <std::string> first_word;
	std::vector <std::string> tmp_iterator = text_vector;
	bool flag_duplicate_port = false;
	for(std::vector <std::string>::iterator it = tmp_iterator.begin();it != tmp_iterator.end();it++)
	{
		tmp.push_back(*it);
		first_word = split_line(tmp);
		
		std::string my_str = first_word[0];
		my_str.erase(std::remove(my_str.begin(), my_str.end(), '\t'), my_str.end());
		check_directives_server(my_str);
		
		if (my_str == "port")
		{

			if (flag_duplicate_port == true)
			{
				std::string errorMessage = std::string("🔴  FAILURE Wrong config_file formated (duplicate port value)");
				throw std::runtime_error(errorMessage);
			}
			flag_duplicate_port = true;
		}
		if (my_str == "server")
			flag_duplicate_port = false;
		
		tmp.clear();
       	first_word.clear();
    }
	std::cout << "🟢  SUCCESS All instructions in config_file are valid directives" << std::endl;
}


int     check_server_block(std::vector<std::string> server_block)
{
    int         init_bracket = 0;
    int         end_bracket = 0;
    std::string str;

    for (std::vector<std::string>::iterator it = server_block.begin(); it != server_block.end(); it++) 
	{
        str = *it;
        if (str.find('{') != std::string::npos)
            init_bracket++;
        if (str.find('}') != std::string::npos)
            end_bracket++;
    }
    if (init_bracket == end_bracket && init_bracket != 0)
        return (1);
    return (0);
}


int    location_block_size(vector_iterator it, vector_iterator end)
{
    for (int i = 0; it != end; it++)
	{
        std::string str = *it;
        if (str.find("}") != std::string::npos)
            return (i);
        i++;
    }
    return (0);
}


std::vector<std::string> parser_vec(const std::string &str)
{
	std::vector <std::string>	vector;
	size_t						start = str.find_first_not_of(' ');
	size_t						pos = str.find_first_of(' ', start);
	std::string					tmp = str.substr(pos + 1);
	std::string					value;

	while ((pos = tmp.find_first_of(' ')) != std::string::npos)
	{
		value = tmp.substr(0, pos);
		vector.push_back(value);
		tmp = tmp.substr(pos + 1);
	}
	vector.push_back(tmp);
	return (vector);
}

std::string	parser_str(const std::string &str)
{
	size_t start = str.find_first_not_of(' ');
	size_t pos = str.find_first_of(' ', start);
	return (str.substr(pos + 1));
}

int parser_num(const std::string &str)
{
	size_t start = str.find_first_not_of(' ');
	size_t pos = str.find_first_of(' ', start);
	std::string temp = str.substr(pos + 1);
	return (std::stoi(temp.c_str()));
}

void parser_fail(const std::string &str)
{
	if (str == "0")
		return;
}
