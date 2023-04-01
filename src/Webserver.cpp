#include "Webserver.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Parser_config_file.hpp"
#include "AddressInfo.hpp"
#include "HttpService.hpp"

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
    Server                      server;

    input_data.open(config_file);
	
	while(getline(input_data,text))
	{
		text_vector.push_back(text);
	
		if (check_server_block(text_vector))
		{
			server.create_server(text_vector);
			_server.push_back(server);
			text_vector.clear();
		}
	}
	print_config_data();
	inspect_config_data();
}

void Webserver::print_config_data()
{
	std::vector <std::string> port_list;
    for (std::vector<Server>::iterator it = _server.begin(); it != _server.end(); it++)
	{
        Server srv_data = *it;

        std::cout << "\n";
        std::cout << "💻  SERVER DATA\n";

		std::cout << "Port > " << srv_data.get_port() << std::endl;
		port_list.push_back(srv_data.get_port());
		unsigned long i;
		for (i = 1; i < port_list.size(); i++) 
		{
  			if (srv_data.get_port() == port_list[i-1])
			{
				std::string errorMessage = std::string("🔴  FAILURE Port in config_file is duplicate in different servers. It is not allowed to use the same port number.");
				throw std::runtime_error(errorMessage);
  			}
		}

        std::cout << "Host > " << srv_data.get_host() << std::endl;
        std::cout << "Server name > " << srv_data.get_server_name() << std::endl;
        std::cout << "Root > " << srv_data.get_root() << std::endl;
        std::cout << "Error page > " << srv_data.get_error_page() << std::endl;
        std::vector<std::string> cgi = srv_data.get_cgi_file_ext();
        std::cout << "CGI files allowed > ";
        for (std::vector<std::string>::iterator it1 = cgi.begin(); it1 != cgi.end(); it1++)
		{
            std::string cgi = *it1;
            std::cout << cgi << " ";
        }
        std::cout << "\n";
        std::cout << "Autoindex > " << srv_data.get_autoindex() << std::endl;
		std::cout << "Max body size > " << srv_data.get_max_body_size() << std::endl;

        std::vector<Location> locations = srv_data.get_location_blocks();
        for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
		{
            Location location = *it;

            std::cout << "\n";
            std::cout << "📂  LOCATION BLOCK DATA\n";
            std::cout << "Location > " << location.get_location() << std::endl;
            std::cout << "Alias > " << location.get_alias() << std::endl;
            std::vector<std::string> methods = location.get_method();
            std::cout << "Accepted methods > ";
            for (std::vector<std::string>::iterator it1 = methods.begin(); it1 != methods.end(); it1++)
			{
                std::string method = *it1;
                std::cout << method << " ";
            }
            std::cout << "\n";
            std::cout << "Index > " << location.get_index() << std::endl;
        }
        std::cout << "\n\n";
    }
}

void    Webserver::inspect_config_data()
{
    for (std::vector<Server>::iterator it = _server.begin(); it != _server.end(); it++)
	{
        Server srv_data = *it;

		//Parsing PORT
        std::string port = srv_data.get_port();
		if (port == "")
		{
			std::string errorMessage = std::string("🔴  FAILURE No port in config_file. Port number is required");
			throw std::runtime_error(errorMessage);
		}
		int port_int = parser_num(port);
       	if (port_int < 0)
		{
			std::string errorMessage = std::string("🔴  FAILURE Invalid port in config_file. Port with negative number is not allowed");
			throw std::runtime_error(errorMessage);
		}
       	if (port_int >= 0 && port_int <= 1024 )
		{
			std::string errorMessage = std::string("🔴  FAILURE Invalid port in config_file. Ports numbers from 0 to 1024 are reserved for privileged services");
			throw std::runtime_error(errorMessage);
		}
       	if (port_int > 63536)
		{
			std::string errorMessage = std::string("🔴  FAILURE Invalid port in config_file. Maximum allowed port number is 65536");
			throw std::runtime_error(errorMessage);
		}

		//Parsing HOST
//        std::string host = srv_data.get_host();
//		if (host != "localhost")
//		{
//			std::string errorMessage = std::string("🔴  FAILURE Invalid host: 'localhost' is the only valid host in 42 network");
//			throw std::runtime_error(errorMessage);
//		}

		//Parsing SERVER_NAME
		std::string server_name = it->_server_name;
		if (server_name.empty())
		{
			std::string errorMessage = std::string("🔴  FAILURE Server name does not exist in config_file");
			throw std::runtime_error(errorMessage);
		}

		//Parsing ROOT
		std::string root_path = it->_root;
		if (root_path.empty())
		{
			std::string errorMessage = std::string("🔴  FAILURE Root path does not exist in config_file");
			throw std::runtime_error(errorMessage);
		}
		int spaces_root = std::count(root_path.begin(), root_path.end(), ' ');
		if (spaces_root > 0)
		{
			std::string errorMessage = std::string("🔴  FAILURE Root path is not a valid path");
			throw std::runtime_error(errorMessage);
		}

		//Parsing ERROR PAGE
		std::string error_page_path = it->_error_page;
		if (error_page_path.empty())
		{
			std::string errorMessage = std::string("🔴  FAILURE Error page path does not exist in config_file");
			throw std::runtime_error(errorMessage);
		}
		int spaces_error_page = std::count(error_page_path.begin(), error_page_path.end(), ' ');
		if (spaces_error_page > 0)
		{
			std::string errorMessage = std::string("🔴  FAILURE Error page path is not a valid path");
			throw std::runtime_error(errorMessage);
		}

		if ( error_page_path.substr(error_page_path.size() - 5) != ".html")
		{
			std::string errorMessage = std::string("🔴  FAILURE Error page path is not a valid path. Is not a valid file.");
			throw std::runtime_error(errorMessage);
		}






		//Parsing CGI FILE EXTENSION
        std::vector<std::string> cgi = it->get_cgi_file_ext();
        for (std::vector<std::string>::iterator it1 = cgi.begin(); it1 != cgi.end(); it1++)
		{
        	std::string cgi_ext = *it1;
			if (cgi_ext != ".py" && cgi_ext != ".sh")
			{
				std::string errorMessage = std::string("🔴  FAILURE CGI file extension not allowed in config_file");
				throw std::runtime_error(errorMessage);
			}
        }

		//Parsing ALIAS
        std::vector<Location> locations = srv_data.get_location_blocks();
        for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
		{
        	Location location = *it;

           	std::string alias = location.get_alias();
			int spaces_alias = std::count(alias.begin(), alias.end(), ' ');
			if (spaces_alias > 0)
			{
				std::string errorMessage = std::string("🔴  FAILURE Alias path is not a valid path");
				throw std::runtime_error(errorMessage);
			}
		}
		
		//Parsing METHODS
        for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
		{
            Location location = *it;

            std::vector<std::string> methods = location.get_method();
            for (std::vector<std::string>::iterator it1 = methods.begin(); it1 != methods.end(); it1++)
			{
                std::string method = *it1;
				if (method != "GET" && method != "POST" && method!= "DELETE" && method != "HEAD")
				{
					std::string errorMessage = std::string("🔴  FAILURE Method not allowed in config_file");
					throw std::runtime_error(errorMessage);
				}
            }
		}
	}
}


std::vector<Server> &Webserver::get_server()
{
	return _server;
}
