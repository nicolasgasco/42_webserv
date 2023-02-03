#include "src/AddressInfo.hpp"
#include "src/Socket.hpp"
#include "src/SocketConnection.hpp"
#include "src/ServerConnection.hpp"

int main()
{
    AddressInfo addr_info;

    Socket socket(addr_info);

    SocketConnection sock_connection(socket.get_socket_id(), addr_info, 10);

    // This while loop should probably be in a Server class
    while (true)
    {
        ServerConnection serv_connection(socket.get_socket_id(), addr_info.get_serv_info());
    }
}