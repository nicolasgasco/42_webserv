#include "AddressInfo.hpp"
#include "Configuration.hpp"
#include "HttpRequest.hpp"
#include "RouterService.hpp"
#include "Socket.hpp"
#include "SocketConnection.hpp"
#include "ServerConnection.hpp"

#include <sys/select.h>

// TODO replace with parameter or default value
#define BACKLOG 10
#define MIN_FD 3

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
			Config config;
			RouterService router;

			if (argc == 1)
			{
				char *config_file_path = (char *)"./config/default.conf";
				config.check_config_file(config_file_path);
			}
			else
				config.check_config_file(argv[1]);

			AddressInfo addr_info;

			Socket socket(addr_info);
			int server_socket = socket.get_socket_id();

			SocketConnection sock_connection(server_socket, addr_info, BACKLOG);

			fd_set ready_fds, current_fds;
			FD_ZERO(&ready_fds);
			FD_ZERO(&current_fds);
			FD_SET(server_socket, &current_fds);
			int max_fd = server_socket;

			// TODO include this loop in a Server class?
			while (true)
			{
				ready_fds = current_fds;

				if (select(max_fd + 1, &ready_fds, NULL, NULL, NULL) < 0)
					std::cerr << std::strerror(errno) << std::endl;

				for (int i = MIN_FD; i <= max_fd; ++i)
				{
					bool isFdReady = FD_ISSET(i, &ready_fds);
					if (isFdReady)
					{
						ServerConnection serv_connection(router);

						bool isFdServer = i == server_socket;
						if (isFdServer)
						{
							int client_fd = serv_connection.accept_connection(i, addr_info.get_serv_info());
							FD_SET(client_fd, &current_fds);

							max_fd = (client_fd > max_fd) ? client_fd : max_fd;
						}
						else
						{
							serv_connection.handle_connection(i);
							FD_CLR(i, &current_fds);
						}
					}
				}
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << "🔴  Failed -> " << e.what() << std::endl;
			return (-1);
		}
	}
	return (0);
}
