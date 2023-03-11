#pragma once

#include <iostream>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

#include "macros.hpp"

// Structs for reference
//
// struct addrinfo
// {
//     int ai_flags;             // AI_PASSIVE, AI_CANONNAME, etc.
//     int ai_family;            // AF_INET, AF_INET6, AF_UNSPEC
//     int ai_socktype;          // SOCK_STREAM, SOCK_DGRAM
//     int ai_protocol;          // use 0 for "any"
//     size_t ai_addrlen;        // size of ai_addr in bytes
//     struct sockaddr *ai_addr; // struct sockaddr_in or _in6
//     char *ai_canonname;       // full canonical hostname

//     struct addrinfo *ai_next; // linked list, next node
// };
//
// struct sockaddr
// {
//     unsigned short sa_family; // address family, AF_xxx
//     char sa_data[14];         // 14 bytes of protocol address
// };
//
// sockaddr_in can be casted to sockaddr and vice-versa
// struct sockaddr_in
// {
//     short int sin_family;        // Address family, AF_INET
//     unsigned short int sin_port; // Port number
//     struct in_addr sin_addr;     // Internet address
//     unsigned char sin_zero[8];   // Same size as struct sockaddr
// };
//
// struct in_addr {
//     uint32_t s_addr; // that's a 32-bit int (4 bytes)
// };

class AddressInfo
{
private:
    struct addrinfo *_serv_info;

public:
    AddressInfo(std::string const &port, std::string const &host_name);
    ~AddressInfo();

    struct addrinfo *get_serv_info() const;

private:
    void _fill_addr_info(std::string const &port, std::string const &host_name);
    struct addrinfo _fill_hints();
    void _check_addr_info_status(int const &status) const;
};
