#!/bin/bash
clear
gcc main.c -o main
./main
gcc -g -Wall -Wextra -Wpedantic main.c
