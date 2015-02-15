#!/bin/sh
valgrind  --leak-check=full --show-leak-kinds=all -v ./wrdo v
