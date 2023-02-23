#pragma once

#include "CgiService.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "macros.hpp"
#include "RouterService.hpp"
#include "Socket.hpp"

#include <fcntl.h>
#include <unistd.h>

class ServerConnection
{
private:
    RouterService const &_router;
    CgiService const &_cgi;
    int _new_sock_id;
    int _bytes_received;
    int _bytes_sent;

public:
    ServerConnection(RouterService const &router, CgiService const &cgi);
    ~ServerConnection();

    int const &accept_connection(int const &sock_id, addrinfo *addr_info);
    void handle_connection(int const &client_fd);

    int const &get_new_sock_id() const;

private:
    void _receive_req(int const &client_fd, HttpRequest &req);
    void _send_res(int const &client_fd, HttpRequest &req);
};