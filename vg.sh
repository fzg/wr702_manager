#!/bin/sh
valgrind  --leak-check=full --show-leak-kinds=all --track-origins=yes -v ./bin/wrdo_x86_64-linux-gnu  -v ip 192.168.0.22 auth toor root mode 1 ssid TRUC psk LOLilOL rpsk rssid rpsk
