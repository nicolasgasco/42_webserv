#pragma once

#include <sys/socket.h>

#include "AddressInfo.hpp"
#include "Socket.hpp"

#define BACKLOG_DEFAULT 10

#define YELLOW "\033[0;33m"
#define NC "\033[0m"

class SocketConnection
{
public:
    SocketConnection();
    SocketConnection(int const &sock_id, AddressInfo &addr_info, int const &backlog);
    ~SocketConnection();

private:
    void _bind_and_listen(int const &sock_id, AddressInfo &addr_info, int const &backlog);
    void _check_bind_result(int const &status, int const &sock_id) const;
    void _check_listen_result(int const &status) const;
};