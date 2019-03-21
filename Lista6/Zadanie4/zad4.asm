section .data
  message:      db "Wprowadz liczbe (max 34)", 10, 0
  scan_format:  db "%d", 0
  print_format: db "Wynik w hex: %08x%08x%08x%08x", 10, 0 ;dziala tylko dla szestnakstkowych liczb

section .bss
  var     resb 4
  result  resb 16

section .text
          global  main
          extern  printf
          extern  scanf

  main:
          push  message
          call  printf
          add   esp, 4

          push  var
          push  scan_format
          call  scanf
          add   esp, 8

          mov   ecx, [var]
          mov   dword [result], 1
          movd  xmm3, dword [result]

          xorps xmm4, xmm4    ;czyszczenie
          xorps xmm5, xmm5
          xorps xmm6, xmm6
          mov   eax, 0x0f
          movd  xmm0, eax

  petla:


          movd      xmm2, ecx


          pmuludq   xmm3, xmm2        ;mnozenie kazdej liczby (32 bity) przez iterator
          pmuludq   xmm4, xmm2        ;wynik mnozenia jest 64 bitowy
          pmuludq   xmm5, xmm2
          pmuludq   xmm6, xmm2



          movdqu    xmm1, xmm3        ;przenoszenie nadmiaru do kolejnego rejestru
          psrlq     xmm1, 32
          paddq     xmm4, xmm1


          movdqu    xmm1, xmm4
          psrlq     xmm1, 32
          paddq     xmm5, xmm1

          movdqu    xmm1, xmm5
          psrlq     xmm1, 32
          paddq     xmm6, xmm1



          cmp   ecx, 1
          je    print


          loop petla

  print:
          movdqu    oword [result], xmm6
          movdqu    oword [result+4], xmm5
          movdqu    oword [result+8], xmm4
          movdqu    oword [result+12], xmm3


          push      dword [result + 12]
          push      dword [result + 8]
          push      dword [result + 4]
          push      dword [result]

          push      print_format
          call      printf
          add       esp, 20
          ret
