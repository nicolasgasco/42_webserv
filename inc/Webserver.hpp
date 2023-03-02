#pragma once

#include "Server.hpp"

class Webserver
{
	public:
    	Webserver();
    	~Webserver();

    	void   	load_config_file(char *config_file);
    	void    print_config_data();
		void	inspect_config_data();
		std::string	bind_socket(std::string *port);

	private:
    	std::vector<Server>     _server;
};
