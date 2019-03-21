

section .data
  message:
        db    "Wprowadz wyrazenie arytmetyczne", 10, 0
  scan_format:
        db    "%lf %c %lf", 0
  print_format:
        db    "%lf", 10, 0

section .bss
  var1:   resb  8
  var2:   resb  8
  sign:   resb  1

section .text
        global    main
        extern    printf
        extern    scanf

  main:
        push  var2
        push  sign
        push  var1
        push  scan_format
        call  scanf
        add   esp, 16


  choose:
        fld   qword [var1]

        cmp   byte [sign], '+'
        je    dodawanie              ;skok na dodawanie

        cmp   byte [sign], '-'
        je    odejmowanie              ;skok na odejmowanie

        cmp   byte [sign], '*'
        je    mnozenie              ;skok na mnozenie

        cmp   byte [sign], '/'
        je    dzielenie              ;skok na dzielenie

  dodawanie:
        fadd  qword [var2]
        call  print
        ret

  odejmowanie:
        fsub  qword [var2]
        call  print
        ret

  mnozenie:
        fmul  qword [var2]
        call  print
        ret

  dzielenie:
        fdiv  qword [var2]
        call  print
        ret



  print:
        fst   qword [var1]
        push  dword [var1+4]
        push  dword [var1]
        push  print_format
        call  printf
        add   esp, 16
        ret
