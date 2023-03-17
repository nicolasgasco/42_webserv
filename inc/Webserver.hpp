#pragma once

#include "Server.hpp"

class AddressInfo;
class ServerConnection;
class HttpService;
class HttpRequest;

class Webserver
{
	friend class AddressInfo;
	friend class ServerConnection;
	friend class HttpService;
	friend class HttpRequest;

	public:
    	Webserver();
    	~Webserver();

    	void   					load_config_file(char *config_file);
    	void    				print_config_data();
		void					inspect_config_data();
		std::vector<Server> 	&get_server();

	private:
    	std::vector<Server>     _server;
};
