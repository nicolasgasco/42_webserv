#pragma once

#include "Server.hpp"

class AddressInfo;
class ServerConnection;
class HttpService;

class Webserver
{
	friend class AddressInfo;
	friend class ServerConnection;
	friend class HttpService;

	public:
    	Webserver();
    	~Webserver();

    	void   	load_config_file(char *config_file);
    	void    print_config_data();
		void	inspect_config_data();
//		std::string	bind_socket(std::string *port);
		std::vector<Server> &get_server();

	private:
    	std::vector<Server>     _server;
};
