#pragma once

#include "AddressInfo.hpp"

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
    void _check_socket_id(int status);
};