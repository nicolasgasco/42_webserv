#include "AddressInfo.hpp"
#include "Webserver.hpp"
#include "Server.hpp"

AddressInfo::AddressInfo(std::string const &port, std::string const &host_name)
{
    this->_fill_addr_info(port, host_name);
}

AddressInfo::~AddressInfo()
{
    freeaddrinfo(this->_serv_info);
}

struct addrinfo *AddressInfo::get_serv_info() const
{
    return this->_serv_info;
}

void AddressInfo::_fill_addr_info(std::string const &port, std::string const &host_name)
{
    struct addrinfo hints = this->_fill_hints();
	std::string host_empty = "";

    std::string host_name_copy = (host_name.empty() || host_name == "localhost") ? host_empty : host_name;
    int status = getaddrinfo(host_name_copy.c_str(), port.c_str(), &hints, &(this->_serv_info));

    this->_check_addr_info_status(status);
}

struct addrinfo AddressInfo::_fill_hints()
{
    struct addrinfo hints = {0, 0, 0, 0, 0, 0, 0, 0};

    hints.ai_family = AF_UNSPEC;     // works both for IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // IP automatically filled

    return hints;
}

void AddressInfo::_check_addr_info_status(int const &status) const
{
    if (status != 0)
    {
        std::string errorMessage;
        errorMessage = "🔴  FAILURE getaddrinfo error: ";
        errorMessage += gai_strerror(status);
        throw std::runtime_error(errorMessage);
    }
    else
        std::cout << YELLOW << "addrinfo list created..." << NC << std::endl;
}
