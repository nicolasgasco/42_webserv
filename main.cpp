#include "src/AddressInfo.hpp"
#include "src/Socket.hpp"
#include "src/SocketConnection.hpp"

int main()
{
    AddressInfo addr_info;

    Socket socket(addr_info);

    SocketConnection sock_connection(socket.get_socket_id(), addr_info, 10);

    while (1)
    {
        socklen_t addr_info_size = sizeof addr_info;
        int new_fd = accept(socket.get_socket_id(), (struct sockaddr *)&addr_info, &addr_info_size);
        std::cout << "Accepting connection..." << std::endl;

        std::string msg = "<!DOCTYPE HTML PUBLIC '-//IETF//DTD HTML 2.0//EN'><html><head><title>Hello world</title></head><body><h1>Welcome to my test page</h1><p>Hello world!</p></body></html>";
        int bytes_sent = send(new_fd, msg.c_str(), msg.length(), 0);
        std::cout << "Bytes sent: " << bytes_sent << std::endl;

        char buf[1000];
        int bytes_received = recv(new_fd, (void *)buf, 500, 0);
        std::cout << "Bytes received: " << bytes_received << std::endl;
        std::cout << buf << std::endl;
    }
}