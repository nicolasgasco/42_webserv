#include "src/AddressInfo.hpp"
#include "src/Socket.hpp"
#include "src/SocketConnection.hpp"
#include "src/ServerConnection.hpp"

// TODO replace with parameter or default value
#define BACKLOG 10
int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

    AddressInfo addr_info;

    Socket socket(addr_info);

    SocketConnection sock_connection(socket.get_socket_id(), addr_info, BACKLOG);

    // TODO include this loop in a Server class?
    while (true)
    {
        ServerConnection serv_connection(socket.get_socket_id(), addr_info.get_serv_info());
    }
}
