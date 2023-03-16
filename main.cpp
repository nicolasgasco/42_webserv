#include "AddressInfo.hpp"
#include "HttpRequest.hpp"
#include "RouterService.hpp"
#include "Socket.hpp"
#include "SocketConnection.hpp"
#include "ServerConnection.hpp"
#include "Config.hpp"
#include "Webserver.hpp"
#include "Server.hpp"

#include <sys/select.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cout << "❌  WRONG USAGE!!!.\n⚠️   Use: ./webserv [configuration file]" << std::endl;
		return (-1);
	}
	else
	{
		try
		{
			CgiService cgi;
			Config config;
			HttpService http;
			RouterService router;

			if (argc == 1)
			{
				char *config_file_path = (char *)"./config/default.conf";
				config.process_config_file(config_file_path);
			}
			else
			{
				config.process_config_file(argv[1]);
			}

			Webserver webserver;
			webserver.load_config_file(argv[1]);

			// Initialize fd_sets for select
			fd_set read_fds, read_fds_cpy, write_fds, write_fds_cpy;
			FD_ZERO(&read_fds);
			FD_ZERO(&read_fds_cpy);
			FD_ZERO(&write_fds);
			FD_ZERO(&write_fds_cpy);

			// Set timeout for select
			struct timeval timeout;
			timeout.tv_sec = SERVER_TIMEOUT;
			timeout.tv_usec = 0;

			int max_fd = STDERR_FILENO + 1;

			// To track active sockets for select
			std::vector<int> active_sockets;

			for (std::vector<Server>::iterator it = webserver.get_server().begin(); it != webserver.get_server().end(); it++)
			{
				std::string port = it->get_port();
				std::string host_name = it->get_host();

				AddressInfo addr_info(port, host_name);
				it->set_addr_info(*addr_info.get_serv_info());

				Socket socket(addr_info);
				active_sockets.push_back(socket.get_socket_id());
				it->set_socket(socket.get_socket_id());

				SocketConnection sock_connection(it->get_socket(), addr_info, BACKLOG_DEFAULT);
				std::cout << YELLOW << "Socket listening on port " << it->get_socket() << "..." << NC << std::endl;

				// Set server_socket for reading
				FD_SET(it->get_socket(), &read_fds_cpy);

				max_fd = it->get_socket() > max_fd ? it->get_socket() : max_fd;
			}

			// For each fd from fd_set: connection + req + res + pointer to server
			std::vector<ServerConnection> connections(max_fd + 1, ServerConnection());
			std::vector<HttpRequest> requests(max_fd + 1, HttpRequest());
			std::vector<HttpResponse> responses(max_fd + 1, HttpResponse(router));
			std::vector<Server *> servers(max_fd + 1, nullptr);

			while (true)
			{
				// Necessary because select is destructive
				read_fds = read_fds_cpy;
				write_fds = write_fds_cpy;

				if (select(max_fd + 1, &read_fds, &write_fds, NULL, &timeout) < 0)
					std::cerr << strerror(errno) << std::endl;

				for (int i = MIN_FD; i <= max_fd; ++i)
				{
					// If fd is set for reading
					if (FD_ISSET(i, &read_fds))
					{
						for (std::vector<Server>::iterator it = webserver.get_server().begin(); it != webserver.get_server().end(); it++)
						{
							bool is_active_socket = std::find(active_sockets.begin(), active_sockets.end(), i) != active_sockets.end();

							// IF fd is socket connection
							if (i == it->get_socket())
							{
								int client_fd = connections.at(i).accept_connection(i, it->get_addr_info());
								FD_SET(client_fd, &read_fds_cpy);

								if (client_fd > max_fd)
								{
									max_fd = client_fd;

									connections.push_back(ServerConnection());
									requests.push_back(HttpRequest());
									responses.push_back(HttpResponse(router));
									servers.push_back(nullptr);
								}

								// Save a pointer to the server for the client
								servers.at(client_fd) = &(*it);
							}
							// If fd is new accepted connection (but not an active socket)
							else if (!is_active_socket)
							{
								connections.at(i).receive_req(i, requests.at(i), servers.at(i));

								if (connections.at(i).get_has_err())
								{
									FD_CLR(i, &read_fds_cpy);
									close(i);
									requests.at(i).reset();
									responses.at(i).reset();
									connections.at(i).reset();
								}
								else if (connections.at(i).get_read_done())
								{
									FD_CLR(i, &read_fds_cpy);
									responses.at(i).build_response(requests.at(i), http, cgi, servers.at(i));
									FD_SET(i, &write_fds_cpy);
									break; // to avoid double reading
								}
							}
						}
					}
					// If fd is set for writing
					else if (FD_ISSET(i, &write_fds))
					{
						connections.at(i).send_res(i, responses.at(i));
						if (connections.at(i).get_has_err() || connections.at(i).get_send_done())
						{
							FD_CLR(i, &write_fds_cpy);
							close(i);
							requests.at(i).reset();
							responses.at(i).reset();
							connections.at(i).reset();
						}
					}
				}
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << "🔴  Failed -> " << e.what() << std::endl;
		}
	}
	return (0);
}
