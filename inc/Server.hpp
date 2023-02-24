#pragma once

#include "Parser_config_file.hpp"
#include "Location.hpp"

#include <iostream>
#include <vector>

class Server
{
	public:
    	Server();
    	~Server();
    
		void	create_server(std::vector<std::string> &server_config);
    	void	reset_server();

    	int		identify_server_value(const std::string &str);
    	enum	server_values
		{ 
			port_ = 0, 
			host_ = 1, 
			server_name_ = 2, 
			error_page_ = 3,
        	cgi_file_ext_ = 4, 
			failed_ = 5, 
			location_ = 6 
		};

    	void    configure_port(const std::string &str);
    	void    configure_host(const std::string &str);
    	void    configure_server_name(const std::string &str);
    	void    configure_cgi_file_ext(const std::string &str);
    	void    configure_error_page(const std::string &str);
    	void    failed_element(const std::string &str);

    	int                     get_port();
    	std::string             get_host();
    	std::string             get_server_name();
    	std::string             get_error_page();
    	std::string             get_cgi_file_ext();
    	std::vector<Location>   get_location_blocks();

    	typedef     std::vector<std::string>::iterator vector_iterator;
    	typedef     void (Server::*Configure)(const std::string &);

	private:
    	int                     _port;
    	std::string             _host;
    	std::string             _server_name;
    	std::string             _error_page;
    	std::string             _cgi_file_ext;
    	std::vector<Location>	_location_blocks;
};
