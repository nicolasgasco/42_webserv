#include "SocketConnection.hpp"

SocketConnection::SocketConnection()
{
    AddressInfo addr_info;

    Socket socket(addr_info);

    struct addrinfo *serv_info = addr_info.get_serv_info();
    int sock_id = socket.get_socket_id();

    int bind_status = bind(sock_id, serv_info->ai_addr, serv_info->ai_addrlen);
    this->_check_bind_result(bind_status, sock_id);
}

SocketConnection::SocketConnection(int sock_id, AddressInfo &addr_info)
{
    struct addrinfo *serv_info = addr_info.get_serv_info();

    int bind_status = bind(sock_id, serv_info->ai_addr, serv_info->ai_addrlen);
    this->_check_bind_result(bind_status, sock_id);
}

SocketConnection::~SocketConnection()
{
}

void SocketConnection::_check_bind_result(int status, int sock_id)
{
    if (status == -1)
    {
        // To avoid 'Address already in use' error
        int yes = 1;

        if (setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
            std::cerr << "Call to bind failed: setsockopt failed: " << std::strerror(errno) << std::endl;
    }
}