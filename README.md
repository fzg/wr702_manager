# wr702_manager

Little command line tool to control a wr702 device

## Install

apt-get install fortunes-off fortunes python python-pip

pip install pycurl


_Warning:_ using this program may result in offensive SSIDs.

## Implemented
  - factory reset
  - reboot
  - set SSID
  - set mode
  - set PSK (only WPA atm)
  - set credentials
  - set ip/netmask
  - attempt to recover from reset

## TODO

Regarding the C version
the fortunes_off file is just the concatenation of nasty fortune files.
On the first line is the `grep -c "^%$" fortunes_off` (count of fortunes)
Of course 1.8MB is too big for some applications, that's why we could 
preprocess further and only keep unique fitting entries, in a possibly compressed
way. that's on the todo.

Implement more functions:
 - Japan
 - WDS
 - uptime
 - beacon / dtim interval
 - dhcp settings
 - dhcp on/off
 - dhcp reservation
Pretty print survey


