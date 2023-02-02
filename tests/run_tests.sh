#!/bin/sh

DEBUG=""

while getopts d flag
do
    case "${flag}" in
        d) DEBUG="debug";;
    esac
done

clang++ -Wall -Wextra -Werror *.cpp ../src/*.cpp -o tests && ./tests $DEBUG
