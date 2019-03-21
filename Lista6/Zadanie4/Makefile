all: zad4

zad4: zad4.asm
	nasm -f elf32 zad4.asm -o zad4.o
	gcc -m32 zad4.o -o zad4

clean:
	rm -f zad4 *~
