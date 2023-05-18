#!/bin/sh

make re

DEBUG=""

while getopts d flag
do
    case "${flag}" in
        d) DEBUG="debug";;
    esac
done

./tests $DEBUG
