#include "Webserver.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Parser_config_file.hpp"
#include "AddressInfo.hpp"

#include <fstream>

Webserver::Webserver() :
	_server()
{
}

Webserver::~Webserver() {}

void	Webserver::load_config_file(char *config_file)
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
        std::vector<std::string> port = srv_data.get_port();
        std::cout << "Port > ";
        for (std::vector<std::string>::iterator it1 = port.begin(); it1 != port.end(); it1++)
		{
            std::string port = *it1;
            std::cout << port << " ";
        }
        std::cout << "\n";
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

void    Webserver::inspect_config_data()
{
    for (std::vector<Server>::iterator it = _server.begin(); it != _server.end(); it++)
	{
        Server srv_data = *it;
        std::vector<std::string> port = srv_data.get_port();

		for (std::vector<std::string>::iterator it1 = port.begin(); it1 != port.end(); it1++)
		{
            std::string port_str_1 = *it1;
			int port_int = parser_num(port_str_1);
        	if (port_int < 0)
			{
				std::cout << "🔴  FAILURE Invalid port in config_file. Port with negative number is not allowed" << std::endl;
				std::exit(0);
			}
        	if (port_int >= 0 && port_int <= 1024 )
			{
				std::cout << "🔴  FAILURE Invalid port in config_file. Ports numbersfrom 0 to 1024 are reserved for privileged services" << std::endl;
				std::exit(0);
			}
        	if (port_int > 63536)
			{
				std::cout << "🔴  FAILURE Invalid port in config_file. Maximum allowed port number is 65536" << std::endl;
				std::exit(0);
			}

        	bool port_duplicate = false;
			for (std::vector<std::string>::iterator it2 = port.begin(); it2 != port.end(); it2++)
			{
            	std::string port_str_2 = *it2;
				int port_int_2 = parser_num(port_str_2);
           		if (port_int == port_int_2)
				{
               		if (port_duplicate == true)
					{	
						std::cout << "🔴  FAILURE Duplicate port in config_file" << std::endl;
						std::exit(0);
					}
					port_duplicate = true;
           		}
			}

			std::string error_page_location = it->_error_page;
			if (error_page_location.empty())
			{
				std::cout << "🔴  FAILURE Error page path does not exist in config_file" << std::endl;
				std::exit(0);
			}
		}
	}
}


std::string  	Webserver::bind_socket(std::string *port)  
{
    for (std::vector<Server>::iterator it = _server.begin(); it != _server.end(); it++)
	{
        Server srv_data = *it;
        std::vector<std::string> port_data = srv_data.get_port();

		for (std::vector<std::string>::iterator it1 = port_data.begin(); it1 != port_data.end(); it1++)
		{
			*port = *it1;
			std::cout << "port config_file  " << *port << std::endl;
			return(*port);
		}
	}
	return (0);
}
