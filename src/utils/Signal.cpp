#include "Signal.hpp"

#include <iostream>
#include <unistd.h>

void signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        for (int i=4; i < 100; i++)
            close(i);            
        std::cout << "\n✅ CLOSED Webserver" << std::endl;
        exit(0);
    }
}