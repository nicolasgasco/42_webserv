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
			root_ = 0, 
			method_ = 1, 
			index_ = 2, 
			autoindex_ = 3, 
			max_body_size_ = 4, 
			failed_ = 5
		};

    	void    configure_location(const std::string &str);
    	void    configure_root(const std::string &str);
    	void    configure_accepted_method(const std::string &str);
    	void    configure_index(const std::string &str);
    	void    configure_autoindex(const std::string &str);
		void    configure_max_body_size(const std::string &str);
		void    failed_element(const std::string &str);

    	std::string                 get_location();
    	std::string                 get_root();
    	std::vector<std::string>    get_method();
    	std::string                 get_index();
    	bool                        get_autoindex();
    	int                         get_max_body_size();

    	typedef     std::vector<std::string>::iterator vector_iterator;
    	typedef     void (Location::*configure)(const std::string &);

	private:
    	std::string                 _location;
    	std::string                 _root;
    	std::vector<std::string>    _accepted_method;
    	std::string                 _index;
    	bool                        _autoindex;
    	int                         _max_body_size;
};
