all: zad2

zad2: zad2.asm
	nasm -f elf32 zad2.asm -o zad2.o
	gcc -m32 zad2.o -o zad2

clean:
	rm -f zad2 *~
