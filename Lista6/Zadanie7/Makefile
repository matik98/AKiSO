all: arcsinh

arcsinh: arcsinh.asm
	nasm -f elf32 arcsinh.asm -o arcsinh.o
	gcc -m32 arcsinh.o -o arcsinh

clean:
	rm -f arcsinh *~
