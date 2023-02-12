#!/bin/sh

clang++ -Wall -Wextra -Werror -std=c++98 main.cpp src/*.cpp src/utils/*.cpp -o webserv && ./webserv