#fzn - 2015

ARCH	 := x86_64-linux-gnu
#ARCH	 := avr #No sys/types no netinet/net
#ARCH	 := mips-linux-gnu

#CC	 := $(ARCH)-gcc
CC := afl-gcc
STRIP	 := $(ARCH)-strip
PRG	 := ./bin/wrdo_$(ARCH)
SRC	 := ./*.c
CFLAGS   := -g -Os
#CFLAGS   := -Os

$(PRG)	:
	- echo "Always set your buffers to NULL on allocation"

	$(CC) $(CFLAGS) -o$(PRG) $(SRC)
#	$(CC) $(CFLAGS) -o$(PRG)_s $(SRC) -static
#	$(STRIP) -s $(PRG)
#	$(STRIP) -s $(PRG)_s
#	upx $(PRG)
#	upx $(PRG)_s

clean	:
	-echo "clean - nothing to clean"

mrproper: clean
	-rm $(PRG) $(PRG)_s

all	: clean $(PRG)

re	: mrproper all
