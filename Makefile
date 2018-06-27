ASM		= nasm
ASMFLAGS	= -felf64 -g
LINKER 		= ld
FILENAME	= Forth29

all: forth29

forth29: forth29.o util.o
	$(LINKER) -o $(FILENAME) forth29.o util.o

forth29.o: forth29.asm
	$(ASM) $(ASMFLAGS) forth29.asm -o forth29.o

util.o: util.inc util.asm
	$(ASM) $(ASMFLAGS) util.asm -o util.o

clean:
	rm -rf *.o $(FILENAME)

again: clean all
