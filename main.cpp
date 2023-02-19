#include "AddressInfo.hpp"
#include "RouterService.hpp"
#include "Socket.hpp"
#include "SocketConnection.hpp"
#include "ServerConnection.hpp"
#include "Configuration.hpp"

// TODO replace with parameter or default value
#define BACKLOG 10
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

			SocketConnection sock_connection(socket.get_socket_id(), addr_info, BACKLOG);

			// TODO include this loop in a Server class?
			while (true)
			{
				ServerConnection serv_connection;

				int client_fd = serv_connection.accept_connection(socket.get_socket_id(), addr_info.get_serv_info());
				serv_connection.handle_connection(client_fd, router);
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
