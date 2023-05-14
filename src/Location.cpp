#include "Location.hpp"

Location::Location() : 
	_location(), 
	_alias(), 
	_accepted_method(0), 
	_index(),
	_redirect()
{
}

Location::~Location() {}

void Location::create_location(vector_iterator it, vector_iterator end)
{
	configure configure_array[5] =
	{
		&Location::configure_alias,
		&Location::configure_accepted_method,
		&Location::configure_index,
		&Location::configure_redirect,
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

void    Location::reset_location()
{
	_location.clear();
	_alias.clear();
	_accepted_method.clear();
	_index.clear();
	_redirect.clear();
}

int     Location::identify_location_value(const std::string &str)
{
    if (str.find("alias") != std::string::npos)
        return alias_;
    else if (str.find("accepted_method") != std::string::npos)
        return method_;
    else if (str.find("index") != std::string::npos)
        return index_;
	else if (str.find("redirect") != std::string::npos)
		return redirect_;
	return failed_;
}

void    Location::configure_location(const std::string &str)
{
    size_t start = str.find_first_of('/');
    size_t end = str.find_first_of(' ', start);
	_location = str.substr(start, end - start);
}

void    Location::configure_alias(const std::string &str)
{
	_alias = parser_str(str);
}

void    Location::configure_accepted_method(const std::string &str)
{
	_accepted_method = parser_vec(str);
}

void    Location::configure_index(const std::string &str)
{
	_index = parser_str(str);
}

void Location::configure_redirect(const std::string &str)
{
	_redirect = parser_str(str);
}

void    Location::failed_element(const std::string &str) 
{
	parser_fail(str);
}

std::string Location::get_location()
{
	return _location;
}

std::string Location::get_alias()
{
	return _alias;
}

std::vector<std::string> Location::get_method()
{
	return _accepted_method;
}

std::string Location::get_index()
{
	return _index;
}

std::string Location::get_redirect()
{
	return _redirect;
}