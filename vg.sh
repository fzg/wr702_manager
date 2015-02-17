#!/bin/sh
valgrind  --leak-check=full --show-leak-kinds=all --track-origins=yes -v ./wrdo_x86_64-linux-gnu v
