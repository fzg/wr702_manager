#fzn - 2015

ARCH	 := x86_64-linux-gnu
#ARCH	 := avr #No sys/types no netinet/net
#ARCH	 := mips-linux-gnu

CC	 := $(ARCH)-gcc
STRIP	 := $(ARCH)-strip
PRG	 := wrdo_$(ARCH)
SRC	 := ./*.c
CFLAGS   := -g -Os
#CFLAGS   := -Os

$(PRG)	:
	$(CC) $(CFLAGS) -o$(PRG) $(SRC)
	$(CC) $(CFLAGS) -o$(PRG)_s $(SRC) -static
	$(STRIP) -s $(PRG)
	$(STRIP) -s $(PRG)_s
	upx $(PRG)
	upx $(PRG)_s

clean	:
	-echo "clean - nothing to clean"

mrproper: clean
	-rm $(PRG) $(PRG)_s

all	: clean $(PRG)

re	: mrproper all
