#pragma once

#include "RouterService.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Socket.hpp"

#include <fcntl.h>
#include <unistd.h>

// TODO change to parametrized value
#define REC_BUFF_SIZE 3000

#define YELLOW "\033[0;33m"
#define NC "\033[0m"

class ServerConnection
{
private:
    RouterService const &_router;
    int _new_sock_id;
    int _bytes_received;
    int _bytes_sent;

public:
    ServerConnection(RouterService const &router);
    ~ServerConnection();

    int const &accept_connection(int const &sock_id, addrinfo *addr_info);
    void handle_connection(int const &client_fd);

    int const &get_new_sock_id() const;

private:
    void _receive_req(int const &client_fd, HttpRequest &req);
    void _send_res(int const &client_fd, HttpRequest &req);
};