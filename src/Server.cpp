#include "Server.hpp"

Server::Server() : 
		_port(), 
		_host(), 
		_server_name(), 
		_error_page(), 
		_cgi_file_ext(),
		_location_blocks()
{
}

Server::~Server() {}

void Server::create_server(std::vector <std::string>& server_config)
{
	Location	location;
    configure configure_array[6] = 
	{ 
		&Server::configure_port,
        &Server::configure_host,
        &Server::configure_server_name,
        &Server::configure_error_page,
        &Server::configure_cgi_file_ext,
        &Server::failed_element
	};
    reset_server(); 
    for (vector_iterator it = server_config.begin(); it != server_config.end(); it++) 
	{
        int server_value = identify_server_value(*it);
        if (server_value == location_) 
		{
			location.create_location(it, (it + location_block_size(it, server_config.end())));
            _location_blocks.push_back(location);
        }
        else 
		{
            configure function = configure_array[server_value];
            (this->*function)(*it);
        }
    }
}

void    Server::reset_server()
{
    _port.clear();
    _host.clear();
    _server_name.clear();
    _error_page.clear();
    _cgi_file_ext.clear();
    _location_blocks.clear();
}

int     Server::identify_server_value(const std::string &str)
{
    if (str.find("port") != std::string::npos)
        return port_;
    else if (str.find("host") != std::string::npos)
        return host_;
    else if (str.find("server_name") != std::string::npos)
        return server_name_;
    else if (str.find("error_page") != std::string::npos)
        return error_page_;
    else if (str.find("cgi_file_ext") != std::string::npos)
        return cgi_file_ext_;
    else if (str.find("location") != std::string::npos)
        return location_;
    return failed_;
}

void    Server::configure_port(const std::string &str)
{
	_port = parser_vec(str);
}

void    Server::configure_host(const std::string &str)
{
	_host = parser_str(str);
}

void    Server::configure_server_name(const std::string &str)
{
	_server_name = parser_str(str);
}

void    Server::configure_error_page(const std::string &str)
{
	_error_page = parser_str(str);
}

void    Server::configure_cgi_file_ext(const std::string &str)
{
	_cgi_file_ext = parser_str(str);
}

void    Server::failed_element(const std::string &str)
{
	parser_fail(str);
}

std::vector<std::string> Server::get_port()
{
	return _port;
}

std::string	Server::get_host()
{
	return _host;
}

std::string	Server::get_server_name()
{
	return _server_name;
}

std::string	Server::get_error_page()
{
	return _error_page;
}

std::string	Server::get_cgi_file_ext()
{
	return _cgi_file_ext;
}

std::vector<Location> Server::get_location_blocks()
{
	return _location_blocks;
}
