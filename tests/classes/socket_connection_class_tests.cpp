#include "../utils.hpp"

#include "../../src/SocketConnection.hpp"
#include <iostream>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

void socket_connection_class_tests(bool IS_DEBUG)
{
    output_suite_title("SOCKET CONNECTION CLASS");
    {
        std::cout << std::endl
                  << "Accept socket is setup correctly:" << std::endl;
        {
            std::cout.setstate(std::ios_base::failbit);

            AddressInfo addr_info;

            Socket my_socket(addr_info);
            int sock_id = my_socket.get_socket_id();
            // Make non-blocking
            fcntl(sock_id, F_SETFL, O_NONBLOCK);

            SocketConnection sock_connection(sock_id, addr_info, 10);

            struct addrinfo *serv_info = addr_info.get_serv_info();
            socklen_t addr_info_size = sizeof serv_info;

            int new_sock_id = accept(sock_id, (struct sockaddr *)&serv_info, &addr_info_size);

            close(new_sock_id);
            close(sock_id);

            //////////////////////////////////////////////

            struct addrinfo hints;
            struct addrinfo *servinfo;

            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;

            getaddrinfo(NULL, "http", &hints, &servinfo);

            int ref_sock_id = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
            // Make non-blocking
            fcntl(ref_sock_id, F_SETFL, O_NONBLOCK);

            bind(ref_sock_id, servinfo->ai_addr, servinfo->ai_addrlen);

            listen(ref_sock_id, 10);

            socklen_t servinfo_size = sizeof servinfo;
            int new_ref_sock_id = accept(sock_id, (struct sockaddr *)&servinfo, &servinfo_size);

            close(new_ref_sock_id);
            close(ref_sock_id);

            std::cout.clear();
            output_test_assertion("with param constructor", is_strict_equal(new_sock_id, new_ref_sock_id, IS_DEBUG));
        }
    }
}