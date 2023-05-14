#include "Signal.hpp"

#include <csignal>
#include <iostream>

void signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "\n✅ CLOSED Webserver" << std::endl;
        exit(0);
    }
}