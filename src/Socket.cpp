#include "Socket.hpp"

Socket::Socket()
{
    AddressInfo addr_info;
    addrinfo *addr_info_res = addr_info.get_serv_info();

    this->_id = socket(addr_info_res->ai_family, addr_info_res->ai_socktype, addr_info_res->ai_protocol);

    this->_check_socket_id(this->_id);
}

Socket::Socket(AddressInfo &addr_info)
{
    addrinfo *addr_info_res = addr_info.get_serv_info();

    this->_id = socket(addr_info_res->ai_family, addr_info_res->ai_socktype, addr_info_res->ai_protocol);

    this->_check_socket_id(this->_id);
}

Socket::~Socket()
{
}

int Socket::get_socket_id()
{
    return this->_id;
}

void Socket::_check_socket_id(int status)
{
    if (status == -1)
        std::cerr << "Socket sys call failed: " << std::strerror(errno) << std::endl;
}