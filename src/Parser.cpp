#include "Parser.hpp"

#include <sstream>

std::vector <std::string> split_line(std::vector <std::string > text_vector)
{
	std::vector <std::string> words;
	std::vector<std::string> tmp = text_vector;
	for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); ++it)
	{
		std::string line = *it;
		std::stringstream input(line);
		std::string str;		
		while (std::getline(input, str, ' '))
		{
			if (str == "")
				continue;
			words.push_back(str);
        }
    }
    return (words);
}

std::vector <std::string> remove_tabs(std::vector <std::string > words)
{
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
	{
		std::string input = *it;
		it->erase(std::remove(it->begin(), it->end(), '\t'), it->end());
    }
    return (words);
}

void check_file_curly_brackets(std::vector <std::string > words)
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
        std::cout << "🔴  FAILURE Wrong config_file formated (curly_brackets)" << std::endl;
        exit(0);
    }
	std::cout << "🟢  SUCCESS Right config_file formated (curly_brackets)" << std::endl;
}

std::vector <std::string > parser_config_file(std::vector <std::string > text_vector)
{
    std::vector<std::string > words;

    words = split_line(text_vector);
    if (words.size() == 0)
    {
        std::cout << "🔴  FAILURE Wrong config_file formated (empty file)" << std::endl;
        exit(0);
    }
	std::cout << "🟢  SUCCESS Right config_file formated (not empty file)" << std::endl;
	words = remove_tabs(words);
    check_file_curly_brackets(words);
	return (words);
}


void specified_words_server(std::string word)
{
	std::string err;
	err = "🔴  FAILURE: [";
    err += word;
    err += "] directives in config_file is not a valid syntax";
    if (word != "server" && 
		word != "listen" && 
		word != "server_name" && 
		word != "error_page" && 
		word != "client_max_body_size" && 
		word != "location" && 
		word != "root" && 
		word != "index" && 
		word != "limit_except" && 
		word != "autoindex" && 
		word != "cgi" && 
		word != "cgi_path" && 
		word != "{" && 
		word != "}") 
	{
		std::cout << err << std::endl;
		exit(1);    
	}
}

void check_words_config_file(std::vector<std::string > text_vector)
{
	std::vector <std::string> tmp;
	std::vector <std::string> first_word;
	std::vector<std::string> tmp_iterator = text_vector;
	for(std::vector<std::string>::iterator it = tmp_iterator.begin();it != tmp_iterator.end();it++)
	{
		tmp.push_back(*it);
		first_word = split_line(tmp);
		
		std::string my_str = first_word[0];
		my_str.erase(std::remove(my_str.begin(), my_str.end(), '\t'), my_str.end());
		specified_words_server(my_str);
		
		tmp.clear();
       	first_word.clear();
    }
	std::cout << "🟢  SUCCESS All instructions in config_file are right" << std::endl;
}
