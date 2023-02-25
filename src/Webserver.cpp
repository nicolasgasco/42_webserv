#include "Webserver.hpp"
#include "Server.hpp"
#include "Parser_config_file.hpp"
#include "Location.hpp"

#include <fstream>
#include <iostream>

Webserver::Webserver() : _server() {}

Webserver::~Webserver() {}

void    Webserver::load_config_file(char *config_file)
{
	std::ifstream 				input_data;
	std::string 				text;
	std::vector <std::string> 	text_vector;
    std::vector <std::string>   server_block;
    Server                      server;

    input_data.open(config_file);
	
	while(getline(input_data,text))
	{
		text_vector.push_back(text);
	}
	if (check_server_block(text_vector))
	{
		server.create_server(text_vector);
		_server.push_back(server);
		text_vector.clear();
	}
}

void Webserver::print_config_data()
{
    for (std::vector<Server>::iterator it = _server.begin(); it != _server.end(); it++)
	{
        Server srv_data = *it;

        std::cout << "\n";
        std::cout << "💻  SERVER DATA\n";
        std::cout << "Port > " << srv_data.get_port() << std::endl;
        std::cout << "Host > " << srv_data.get_host() << std::endl;
        std::cout << "Server name > " << srv_data.get_server_name() << std::endl;
        std::cout << "Error page > " << srv_data.get_error_page() << std::endl;
        std::cout << "CGI files > " << srv_data.get_cgi_file_ext() << std::endl;

        std::vector<Location> locations = srv_data.get_location_blocks();
        for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
		{
            Location location = *it;

            std::cout << "\n";
            std::cout << "📂  LOCATION BLOCK DATA\n";
            std::cout << "Location > " << location.get_location() << std::endl;
            std::cout << "Root > " << location.get_root() << std::endl;

            std::vector<std::string> methods = location.get_method();
            std::cout << "Accepted methods > ";
            for (std::vector<std::string>::iterator it1 = methods.begin(); it1 != methods.end(); it1++)
			{
                std::string method = *it1;
                std::cout << method << " ";
            }
            std::cout << "\n";
            std::cout << "Index > " << location.get_index() << std::endl;
            std::cout << "Autoindex > " << location.get_autoindex() << std::endl;
			std::cout << "Max body size > " << location.get_max_body_size() << std::endl;
        }
        std::cout << "\n\n";
    }
}
