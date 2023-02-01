#pragma once

#include <sys/socket.h>

#include "AddressInfo.hpp"
#include "Socket.hpp"

class SocketConnection
{
public:
    SocketConnection();
    SocketConnection(int sock_id, AddressInfo &addr_info);

    ~SocketConnection();

private:
    void _check_bind_result(int status, int sock_id);
};