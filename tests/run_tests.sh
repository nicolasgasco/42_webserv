#!/bin/sh

DEBUG=""

while getopts d flag
do
    case "${flag}" in
        d) DEBUG="debug";;
    esac
done

clang++ *.cpp --std=c++11 ../src/*.cpp -o tests && ./tests $DEBUG
