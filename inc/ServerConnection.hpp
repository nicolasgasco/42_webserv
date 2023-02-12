#pragma once

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
    int _new_sock_id;
    int _bytes_received;
    int _bytes_sent;

public:
    ServerConnection();
    ServerConnection(int sock_id, addrinfo *addr_info);

    ~ServerConnection();

    int get_new_sock_id();

private:
    void _accept_recv_send(int sock_id, addrinfo *addr_info);
    void _check_accept_return();
    void _check_recv_return();
    void _check_send_return();
};