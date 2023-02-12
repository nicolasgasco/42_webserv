#include "ServerConnection.hpp"

ServerConnection::ServerConnection()
{
    AddressInfo addr_info;

    Socket socket(addr_info);

    this->_accept_recv_send(socket.get_socket_id(), addr_info.get_serv_info());
}

ServerConnection::ServerConnection(int sock_id, addrinfo *addr_info)
{
    this->_accept_recv_send(sock_id, addr_info);
}

ServerConnection::~ServerConnection()
{
    std::cout << YELLOW << "Connection (" << this->_new_sock_id << ") closed..." << NC << std::endl;
    close(this->_new_sock_id);
}

int ServerConnection::get_new_sock_id()
{
    return this->_new_sock_id;
}

void ServerConnection::_accept_recv_send(int sock_id, addrinfo *addr_info)
{
    socklen_t addr_info_size = sizeof addr_info;
    this->_new_sock_id = accept(sock_id, (struct sockaddr *)addr_info, &addr_info_size);
    this->_check_accept_return();

    HttpRequest req;
    this->_bytes_received = recv(this->_new_sock_id, (void *)req.get_buff(), REC_BUFF_SIZE, 0);
    this->_check_recv_return();

    req.parse_req();
    req.output_status();
    if (!req.has_error())
        return;

    // TODO replace this with real answer
    std::string msg = "<!DOCTYPE HTML PUBLIC '-//IETF//DTD HTML 2.0//EN'><html><head><title>Hello world</title></head><body><h1>Welcome to my test page</h1><p>Hello world!</p></body></html>";
    this->_bytes_sent = send(this->_new_sock_id, msg.c_str(), msg.length(), 0);
    this->_check_send_return();
}

void ServerConnection::_check_accept_return()
{
    if (this->_new_sock_id == -1)
        std::cerr << "Error: accept: " << std::strerror(errno) << std::endl;
    else
        std::cout << YELLOW << std::endl
                  << "New connection accepted (" << this->_new_sock_id << ")..." << NC << std::endl;
}

void ServerConnection::_check_recv_return()
{
    if (this->_bytes_received == -1)
        std::cerr << "Error: recv: " << std::strerror(errno) << std::endl;
    else
        std::cout << YELLOW << "Bytes received: " << this->_bytes_received << NC << std::endl;
}

void ServerConnection::_check_send_return()
{
    if (this->_bytes_sent == -1)
        std::cerr << "Error: send: " << std::strerror(errno) << std::endl;
    else
        std::cout << YELLOW << "Bytes sent: " << this->_bytes_sent << NC << std::endl;
}