#!/bin/sh

DEBUG=""

while getopts d flag
do
    case "${flag}" in
        d) DEBUG="debug";;
    esac
done

./tests $DEBUG
#clang++ *.cpp --std=c++11 ../src/*.cpp ../src/utils/*.cpp -o tests && ./tests $DEBUG
