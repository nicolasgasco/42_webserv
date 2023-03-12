#pragma once

#include "AddressInfo.hpp"
#include "Location.hpp"

#include <iostream>

class Webserver;

class Server
{
	friend class Webserver;
	
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
			root_ = 3, 
			error_page_ = 4,
        	cgi_file_ext_ = 5, 
			autoindex_ = 6, 
			max_body_size_ = 7, 
			failed_ = 8, 
			location_ = 9 
		};

    	void    configure_port(const std::string &str);
    	void    configure_host(const std::string &str);
    	void    configure_server_name(const std::string &str);
    	void    configure_root(const std::string &str);
    	void    configure_error_page(const std::string &str);
    	void    configure_cgi_file_ext(const std::string &str);
    	void    configure_autoindex(const std::string &str);
		void    configure_max_body_size(const std::string &str);
    	void    failed_element(const std::string &str);

    	std::string					get_port();
    	std::string             	get_host();
    	std::string             	get_server_name();
    	std::string             	get_root();
    	std::string             	get_error_page();
		std::vector<std::string>   	get_cgi_file_ext();
    	bool                        get_autoindex();
    	int                         get_max_body_size();
    	std::vector<Location>   	get_location_blocks();
		int							get_socket() const;
		struct addrinfo *			get_addr_info();

		void						set_socket(int const &socket);
		void						set_addr_info(struct addrinfo const &addr_info);

    	typedef     std::vector<std::string>::iterator vector_iterator;
    	typedef     void (Server::*configure)(const std::string &);
		
	private:
		std::string					_port;
    	std::string         	    _host;
    	std::string         	    _server_name;
    	std::string                 _root;
    	std::string        		    _error_page;
    	std::vector<std::string>   	_cgi_file_ext;
    	bool                        _autoindex;
    	int                         _max_body_size;
    	std::vector<Location>		_location_blocks;
		int							_socket;
		struct addrinfo 			_addr_info;
};
