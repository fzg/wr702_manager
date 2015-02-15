#fzn - 2015

PRG	 := wrdo
SRC	 := ./*.c


$(PRG)	:
	gcc -g -O0 -o $(PRG) $(SRC)

clean	:
	-echo "clean"

mrproper: clean
	-rm $(PRG)

all	: clean $(PRG)

re	: mrproper all
