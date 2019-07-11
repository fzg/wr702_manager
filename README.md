# wr702_manager

Little command line tool to control a wr702 device

## Install

apt-get install fortunes-off fortunes


_Warning:_ using this program may result in offensive SSIDs.

## Usage examples
 - ./wrdo ip 10.0.0.1 auth root hackme ssid FreeBeer psk BreeFeer
 - ./wrdo rpsk rssid
 - ./wrdo -v user bob -v -v pass isucksmellypatties
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

Regarding the C version [python version abandoned]

x. Fix operand parsing: "-v -v -v ip -v" sould not be valid, as -v is an operator

x. Add argument parsing rules for missing functionalities

x. Clean up output

x. The fortunes_off file is just the concatenation of big enough words from offensive fortunes..
Of course 430KB is too big for some platforms, that's why we could 
preprocess further and in a possibly compressed way. that's on the todo.

Implement more functions:
 - Japan
 - WDS
 - uptime
 - beacon / dtim interval
 - dhcp settings
 - dhcp on/off
 - dhcp reservation
Pretty print survey


