#include "src/AddressInfo.hpp"
#include "src/Socket.hpp"
#include "src/SocketConnection.hpp"

int main()
{
    AddressInfo addr_info("4200");

    Socket socket(addr_info);

    SocketConnection sock_connection(socket.get_socket_id(), addr_info, 10);
}