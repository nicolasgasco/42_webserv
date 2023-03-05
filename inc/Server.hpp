#pragma once

#include "Location.hpp"

#include <iostream>

class Webserver;

class Server
{
	public:
    	Server();
    	~Server();
		
		friend class Webserver;
    
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
			autoindex_ = 5, 
			max_body_size_ = 6, 
			failed_ = 7, 
			location_ = 8 
		};

    	void    configure_port(const std::string &str);
    	void    configure_host(const std::string &str);
    	void    configure_server_name(const std::string &str);
    	void    configure_error_page(const std::string &str);
    	void    configure_cgi_file_ext(const std::string &str);
    	void    configure_autoindex(const std::string &str);
		void    configure_max_body_size(const std::string &str);
    	void    failed_element(const std::string &str);

    	std::vector<std::string>	get_port();
    	std::string             	get_host();
    	std::string             	get_server_name();
    	std::string             	get_error_page();
    	std::string             	get_cgi_file_ext();
    	bool                        get_autoindex();
    	int                         get_max_body_size();
    	std::vector<Location>   	get_location_blocks();

    	typedef     std::vector<std::string>::iterator vector_iterator;
    	typedef     void (Server::*configure)(const std::string &);
		
	private:
		std::vector<std::string>	_port;
    	std::string         	    _host;
    	std::string         	    _server_name;
    	std::string        		    _error_page;
    	std::string             	_cgi_file_ext;
    	bool                        _autoindex;
    	int                         _max_body_size;
    	std::vector<Location>		_location_blocks;
};
