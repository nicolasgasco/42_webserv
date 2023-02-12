#!/bin/sh

clang++ -Wall -Wextra -Werror main.cpp src/*.cpp src/utils/*.cpp -o webserv && ./webserv