#pragma once

#include "AddressInfo.hpp"

#include <fcntl.h>

class Socket
{
private:
    int _id;

public:
    Socket(AddressInfo const &addr_info);

    ~Socket();

    int get_socket_id() const;

private:
    void _create_socket(AddressInfo const &addr_info);
    void _check_socket_id(int const &status) const;
};