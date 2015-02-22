#fzn - 2015

ARCH	 := x86_64-linux-gnu
#ARCH	 := avr #No sys/types no netinet/net
#ARCH	 := mips-linux-gnu

CC	 := $(ARCH)-gcc
STRIP	 := $(ARCH)-strip
PRG	 := wrdo_$(ARCH)
SRC	 := b64.c main.c net.c
CFLAGS   := -g -Os #-m64
#CFLAGS   := -Os

$(PRG)	:
	$(CC) $(CFLAGS) -c fortunes.c
	$(CC) $(CFLAGS) -o$(PRG) fortunes.o  $(SRC)  -Wl,--format=binary -Wl,f_off -Wl,--format=default
	$(CC) $(CFLAGS) -o$(PRG)_s fortunes.o $(SRC) -static -Wl,--format=binary -Wl,f_off -Wl,--format=default
#	$(STRIP) -s $(PRG)
	$(STRIP) -s $(PRG)_s
#	upx $(PRG)
	upx $(PRG)_s

clean	:
	-echo "clean - nothing to clean"

mrproper: clean
	-rm $(PRG) $(PRG)_s

all	: clean $(PRG)

re	: mrproper all
