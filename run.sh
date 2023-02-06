#!/bin/sh

clang++ -Wall -Wextra -Werror main.cpp src/*.cpp -o webserv && ./webserv