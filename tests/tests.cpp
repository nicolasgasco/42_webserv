#include <iostream>

#include "address_info_class/address_info_class_tests.cpp"

int main(int argc, char *argv[])
{
    bool IS_DEBUG = false;
    if (argc > 1)
        IS_DEBUG = (static_cast<std::string>(argv[1]) == "debug") ? true : false;

    // AddressInfo class
    address_info_class_tests(IS_DEBUG);
}
