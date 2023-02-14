#include "SocketConnection.hpp"

SocketConnection::SocketConnection()
{
    AddressInfo addr_info;

    Socket socket(addr_info);
    int sock_id = socket.get_socket_id();

    this->_bind_and_listen(sock_id, addr_info, BACKLOG_DEFAULT);
}

SocketConnection::SocketConnection(int const &sock_id, AddressInfo &addr_info, int const &backlog)
{
    this->_bind_and_listen(sock_id, addr_info, backlog);
}

SocketConnection::~SocketConnection()
{
}

void SocketConnection::_bind_and_listen(int const &sock_id, AddressInfo &addr_info, int const &backlog)
{
    struct addrinfo *serv_info = addr_info.get_serv_info();

    int bind_status = bind(sock_id, serv_info->ai_addr, serv_info->ai_addrlen);
    this->_check_bind_result(bind_status, sock_id);

    int listen_status = listen(sock_id, backlog);
    this->_check_listen_result(listen_status);
}

void SocketConnection::_check_bind_result(int const &status, int const &sock_id) const
{
    if (status == -1)
    {
        // To avoid 'Address already in use' error
        int yes = 1;
        if (setsockopt(sock_id, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
            std::cerr << "Call to bind failed: setsockopt failed: " << std::strerror(errno) << std::endl;
        else
            std::cout << YELLOW << "Socket connection established..." << NC << std::endl;
    }
    else
        std::cout << YELLOW << "Socket connection established..." << NC << std::endl;
}

void SocketConnection::_check_listen_result(int const &status) const
{
    if (status == -1)
        std::cerr << "Call to listen failed: setsockopt failed: " << std::strerror(errno) << std::endl;
}