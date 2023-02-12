#include "../utils.hpp"

#include "../../inc/AddressInfo.hpp"

#include <iostream>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

void address_info_class_tests(bool IS_DEBUG)
{
    output_suite_title("ADDRESSINFO CLASS");
    {
        std::cout << std::endl
                  << "Struct should be initialized correctly:" << std::endl;
        {
            std::cout.setstate(std::ios_base::failbit);

            AddressInfo info;

            //////////////////////////////////////////////

            struct addrinfo hints;
            struct addrinfo *servinfo;

            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;

            getaddrinfo(NULL, "http", &hints, &servinfo);

            std::cout.clear();
            output_test_assertion("with default constructor", is_strict_equal_addrinfo(*(info.get_serv_info()), *servinfo, IS_DEBUG));
        }
        {
            std::cout.setstate(std::ios_base::failbit);

            std::string port = "4200";

            AddressInfo info(port);

            //////////////////////////////////////////////

            struct addrinfo hints;
            struct addrinfo *servinfo;

            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;

            getaddrinfo(NULL, port.c_str(), &hints, &servinfo);

            std::cout.clear();
            output_test_assertion("with parameter constructor (4200)", is_strict_equal_addrinfo(*(info.get_serv_info()), *servinfo, IS_DEBUG));
        }
    }
}
