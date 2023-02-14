#include "AddressInfo.hpp"

AddressInfo::AddressInfo()
{
    // TODO define default port
    this->_get_addr_info("http");
}

AddressInfo::AddressInfo(std::string const &port)
{
    this->_get_addr_info(port);
}

AddressInfo::~AddressInfo()
{
    freeaddrinfo(this->_serv_info);
}

struct addrinfo *AddressInfo::get_serv_info()
{
    return this->_serv_info;
}

void AddressInfo::_get_addr_info(std::string const &port)
{
    struct addrinfo hints = this->_fill_hints();

    int status = getaddrinfo(NULL, port.c_str(), &hints, &(this->_serv_info));

    this->_check_addr_info_status(status);
}

struct addrinfo AddressInfo::_fill_hints()
{
    struct addrinfo hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // works both for IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // IP automatically filled

    return hints;
}

void AddressInfo::_check_addr_info_status(int const &status)
{
    if (status != 0)
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
    else
        std::cout << YELLOW << "addrinfo list created..." << NC << std::endl;
}