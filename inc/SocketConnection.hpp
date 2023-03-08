#pragma once

#include "AddressInfo.hpp"
#include "macros.hpp"
#include "Socket.hpp"
#include "HttpRequest.hpp"

#include <sys/socket.h>

//class HttpRequest;

class SocketConnection
{
public:
    SocketConnection();
    SocketConnection(int const &sock_id, AddressInfo &addr_info, int const &backlog);
    ~SocketConnection();

	friend class HttpRequest;

private:
    void _bind_and_listen(int const &sock_id, AddressInfo &addr_info, int const &backlog);
    void _check_bind_result(int const &status, int const &sock_id) const;
    void _check_listen_result(int const &status) const;
};
