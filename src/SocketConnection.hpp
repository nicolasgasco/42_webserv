#pragma once

#include <sys/socket.h>

#include "AddressInfo.hpp"
#include "Socket.hpp"

#define BACKLOG_DEFAULT 10

class SocketConnection
{
public:
    SocketConnection();
    SocketConnection(int sock_id, AddressInfo &addr_info, int backlog);
    ~SocketConnection();

private:
    void _check_bind_result(int status, int sock_id);
    void _check_listen_result(int status);
};