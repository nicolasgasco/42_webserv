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
    Socket(AddressInfo &addr_info);

    ~Socket();

    int get_socket_id();

private:
    void _create_socket(AddressInfo &addr_info);
    void _check_socket_id(int status);
};