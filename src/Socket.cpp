#include "Socket.hpp"

Socket::Socket(AddressInfo const &addr_info)
{
    this->_create_socket(addr_info);
}

Socket::~Socket()
{
}

int Socket::get_socket_id() const
{
    return this->_id;
}

void Socket::_create_socket(AddressInfo const &addr_info)
{
    addrinfo *addr_info_res = addr_info.get_serv_info();

    this->_id = socket(addr_info_res->ai_family, addr_info_res->ai_socktype, addr_info_res->ai_protocol);
    fcntl(this->_id, F_SETFL, O_NONBLOCK);

    this->_check_socket_id(this->_id);
}

void Socket::_check_socket_id(int const &status) const
{
    if (status == -1)
        std::cerr << "Socket sys call failed: " << strerror(errno) << std::endl;
    else
        std::cout << YELLOW << "Socket created (" << this->_id << ")..." << NC << std::endl;
}