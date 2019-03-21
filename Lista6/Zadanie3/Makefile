all: zad3

zad3: zad3.asm
	nasm -f elf32 zad3.asm -o zad3.o
	gcc -m32 zad3.o -o zad3

clean:
	rm -f zad3 *~
