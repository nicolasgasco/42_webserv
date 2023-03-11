#pragma once

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
    int _new_sock_id;
    int _bytes_received, _bytes_sent;
    bool _read_done, _sent_done;
    bool _has_err;

public:
    ServerConnection();
    ~ServerConnection();

    bool get_has_err() const;
    int const &get_new_sock_id() const;
    bool get_read_done() const;
    bool get_send_done() const;

    int const &accept_connection(int const &sock_id, addrinfo *addr_info);
    void receive_req(int const &client_fd, HttpRequest &req);
    void send_res(int const &client_fd, HttpResponse &res);

    void reset();
};