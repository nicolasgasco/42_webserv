#pragma once

#include <sys/socket.h>

#include "AddressInfo.hpp"
#include "macros.hpp"
#include "Socket.hpp"

class SocketConnection
{
public:
    SocketConnection(int const &sock_id, AddressInfo &addr_info, int const &backlog);
    ~SocketConnection();

private:
    void _bind_and_listen(int const &sock_id, AddressInfo &addr_info, int const &backlog);
    void _check_bind_result(int const &status, int const &sock_id) const;
    void _check_listen_result(int const &status) const;
};