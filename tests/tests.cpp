#include <iostream>

#include "classes/address_info_class_tests.cpp"
#include "classes/socket_class_tests.cpp"
#include "classes/socket_connection_class_tests.cpp"

int main(int argc, char *argv[])
{
    bool IS_DEBUG = false;
    if (argc > 1)
        IS_DEBUG = (static_cast<std::string>(argv[1]) == "debug") ? true : false;

    address_info_class_tests(IS_DEBUG);
    socket_class_tests(IS_DEBUG);
    socket_connection_class_tests(IS_DEBUG);
}
