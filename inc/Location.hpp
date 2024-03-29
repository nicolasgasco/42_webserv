#pragma once

#include "Parser_config_file.hpp"

#include <string>
#include <vector>

class Location
{
	public:
    	Location();
    	~Location();

    	void    create_location(vector_iterator it, vector_iterator end);
		void    reset_location();

    	int     identify_location_value(const std::string &str);
    	enum	location_values
		{ 
			alias_ = 0, 
			method_ = 1, 
			index_ = 2, 
			redirect_ = 3,
			failed_ = 4
		};

    	void    configure_location(const std::string &str);
    	void    configure_alias(const std::string &str);
    	void    configure_accepted_method(const std::string &str);
		void 	configure_index(const std::string &str);
		void 	configure_redirect(const std::string &str);
		void    failed_element(const std::string &str);

    	std::string                 get_location();
    	std::string                 get_alias();
    	std::vector<std::string>    get_method();
		std::string 				get_index();
		std::string 				get_redirect();

		typedef     std::vector<std::string>::iterator vector_iterator;
    	typedef     void (Location::*configure)(const std::string &);

	private:
    	std::string                 _location;
    	std::string                 _alias;
    	std::vector<std::string>    _accepted_method;
		std::string 				_index;
		std::string 				_redirect;
};
