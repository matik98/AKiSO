
        global main
        extern printf

        section .text



main:
        mov   ebx, 2
        call print

myloop:
        mov   ecx, 2
        cmp   ecx, ecx        ;skok bezwzgledny
        je    compare_loop

myloopcont:
        call  print
        add   ebx, 1
        cmp   ebx, 10000      ; wywolanie petli
        jle    myloop
end:
        ret

compare_loop:
        mov   edx, 0
        mov   eax, ebx
        div   ecx
        cmp   edx, 0
        je    skip
        add   ecx, 1
        mov   eax, ebx
        shr   eax, 1
        cmp   ecx, eax        ; wywolanie petli
        jl    compare_loop

        cmp   ecx, ecx        ;skok bezwzgledny
        je    myloopcont

skip:
        add   ebx, 1
	cmp   ebx, 10000
	jge   end
        cmp   ecx, ecx        ;skok bezwzgledny
        je    myloop

print:
        push  ebx
        push  dword format
        call  printf
        add   esp, 8
        ret

format:
        db    '%d', 10, 0
