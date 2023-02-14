#pragma once

#include "AddressInfo.hpp"

#define YELLOW "\033[0;33m"
#define NC "\033[0m"

class Socket
{
private:
    int _id;

public:
    Socket();
    Socket(AddressInfo const &addr_info);

    ~Socket();

    int get_socket_id() const;

private:
    void _create_socket(AddressInfo const &addr_info);
    void _check_socket_id(int const &status) const;
};