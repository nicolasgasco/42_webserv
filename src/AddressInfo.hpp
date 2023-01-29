#pragma once

#include <iostream>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

class AddressInfo
{
private:
    struct addrinfo *_serv_info;

public:
    AddressInfo();
    AddressInfo(std::string port);
    ~AddressInfo();

    struct addrinfo *get_serv_info();

private:
    struct addrinfo _fill_hints();
    void _check_addr_info_status(int status);
};