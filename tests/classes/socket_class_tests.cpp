#include "../utils.hpp"

#include "../../inc/Socket.hpp"

#include <iostream>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void socket_class_tests(bool IS_DEBUG)
{
    output_suite_title("SOCKET CLASS");
    {
        std::cout << std::endl
                  << "Struct should be initialized correctly:" << std::endl;
        {
            std::cout.setstate(std::ios_base::failbit);

            Socket sock;
            int id = sock.get_socket_id();

            close(id);

            //////////////////////////////////////////////

            struct addrinfo hints;
            struct addrinfo *servinfo;

            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;

            getaddrinfo(NULL, "http", &hints, &servinfo);

            int ref_id = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
            close(ref_id);

            std::cout.clear();
            output_test_assertion("with default constructor", is_strict_equal(id, ref_id, IS_DEBUG));
        }
        {
            std::cout.setstate(std::ios_base::failbit);

            std::string port = "4200";

            AddressInfo info(port);

            Socket sock(info);
            int id = sock.get_socket_id();

            close(id);

            //////////////////////////////////////////////

            struct addrinfo hints;
            struct addrinfo *servinfo;

            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;

            getaddrinfo(NULL, port.c_str(), &hints, &servinfo);

            int ref_id = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
            close(ref_id);

            std::cout.clear();
            output_test_assertion("with param constructor", is_strict_equal(id, ref_id, IS_DEBUG));
        }
    }
}
