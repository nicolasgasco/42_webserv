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
			// Todo check if Config and RouterService should be long lived
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
			webserver.print_config_data();
			webserver.inspect_config_data();

			std::string port;
			webserver.bind_socket(&port);

			// TODO fix this
			// while (webserver.bind_socket(&port) != "")

			std::string server_name;
			webserver.get_server_name(&server_name);

			AddressInfo addr_info(port);

			Socket socket(addr_info);
			int server_socket = socket.get_socket_id();

			SocketConnection sock_connection(server_socket, addr_info, BACKLOG_DEFAULT);

			// Initialize fd_sets for select
			fd_set read_fds, read_fds_cpy, write_fds, write_fds_cpy;
			FD_ZERO(&read_fds);
			FD_ZERO(&read_fds_cpy);
			FD_ZERO(&write_fds);
			FD_ZERO(&write_fds_cpy);

			// Set server_socket for reading
			FD_SET(server_socket, &read_fds_cpy);

			// Set timeout for select
			struct timeval timeout;
			timeout.tv_sec = SERVER_TIMEOUT;
			timeout.tv_usec = 0;

			int max_fd = server_socket;
			// For each socket: connection + req + res
			std::vector<ServerConnection> connections(max_fd + 1, ServerConnection());
			std::vector<HttpRequest> requests(max_fd + 1, HttpRequest());
			std::vector<HttpResponse> responses(max_fd + 1, HttpResponse(router));

			while (true)
			{
				// Necessary because select is destructive
				read_fds = read_fds_cpy;
				write_fds = write_fds_cpy;

				if (select(max_fd + 1, &read_fds, &write_fds, NULL, &timeout) < 0)
					std::cerr << std::strerror(errno) << std::endl;

				for (int i = MIN_FD; i <= max_fd; ++i)
				{
					// If fd is set for reading
					if (FD_ISSET(i, &read_fds))
					{
						// IF fd is socket connection
						if (i == server_socket)
						{
							int client_fd = connections.at(i).accept_connection(i, addr_info.get_serv_info());
							FD_SET(client_fd, &read_fds_cpy);

							if (client_fd > max_fd)
							{
								max_fd = client_fd;

								connections.push_back(ServerConnection());
								requests.push_back(HttpRequest());
								responses.push_back(HttpResponse(router));
							}
						}
						// If fd is new accepted connection
						else
						{
							connections.at(i).receive_req(i, requests.at(i));

							if (connections.at(i).get_read_done())
							{
								responses.at(i).build_response(requests.at(i), http, cgi);

								FD_CLR(i, &read_fds_cpy);
								FD_SET(i, &write_fds_cpy);
							}
						}
					}
					// If fd is set for writing
					else if (FD_ISSET(i, &write_fds))
					{
						connections.at(i).send_res(i, responses.at(i));
						if (connections.at(i).get_send_done())
						{
							FD_CLR(i, &write_fds_cpy);
							close(i);
							requests.at(i).reset();
							responses.at(i).reset();
							connections.at(i).reset();
						}
						// TODO add redirections here
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
