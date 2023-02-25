#include "Location.hpp"

Location::Location() : 
	_location(), 
	_root(), 
	_accepted_method(0), 
	_index(),
	_autoindex(false), 
	_max_body_size(0) 
{
}

Location::~Location() {}

void Location::create_location(vector_iterator it, vector_iterator end)
{
    configure configure_array[6] =
	{
		&Location::configure_root,
      	&Location::configure_accepted_method,
     	&Location::configure_index,
      	&Location::configure_autoindex,
   	    &Location::configure_max_body_size,
 	    &Location::failed_element
	};
    reset_location();
	configure_location(*it);
    for(; it != end; it++)
	{
        int location_values = identify_location_value(*it);
        configure function = configure_array[location_values];
        (this->*function)(*it);
    }
}

int     Location::identify_location_value(const std::string &str){
    if (str.find("root") != std::string::npos)
        return root_;
    else if (str.find("accepted_method") != std::string::npos)
        return method_;
    else if (str.find("autoindex") != std::string::npos)
        return autoindex_;
    else if (str.find("index") != std::string::npos)
        return index_;
    else if (str.find("max_body_size") != std::string::npos)
        return max_body_size_;
	return failed_;
}


void    Location::configure_location(const std::string &str)
{
    size_t start = str.find_first_of('/');
    size_t end = str.find_first_of(' ', start);
	_location = str.substr(start, end - start);
}

void    Location::configure_root(const std::string &str)
{
	_root = parser_str(str);
}

void    Location::configure_accepted_method(const std::string &str)
{
	_accepted_method = parser_vec(str);
}

void    Location::configure_index(const std::string &str)
{
	_index = parser_str(str);
}

void    Location::failed_element(const std::string &str) 
{
	parser_fail(str);
}

void    Location::configure_max_body_size(const std::string &str)
{
	_max_body_size = parser_num(str);
}

void    Location::configure_autoindex(const std::string &str)
{
	if (parser_str(str) == "on")
        _autoindex = true;
}

std::string Location::get_location()
{
	return _location;
}

std::string Location::get_root()
{
	return _root;
}

std::string Location::get_index()
{
	return _index;
}

std::vector<std::string> Location::get_method()
{
	return _accepted_method;
}

int Location::get_max_body_size()
{
	return _max_body_size;
}

bool Location::get_autoindex()
{
	return _autoindex;
}

void    Location::reset_location()
{
	_location.clear();
	_root.clear();
	_accepted_method.clear();
	_index.clear();
	_autoindex = false;
	_max_body_size = 0;
}
