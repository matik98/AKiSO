section .data
  message:
        db    "Wprowadz x", 10, 0
  scan_format:
        db    "%lf", 0
  print_format:
        db    "arcsinhx = %lf", 10, 0

section .bss
  var:  resb  8
  res:  resb  8

section .text
        global    main
        extern    printf
        extern    scanf

  main:
        push  message
        call  printf
        push  var
        push  scan_format
        call  scanf
        add   esp, 8

  count:
        finit
        fld1
        fld   qword[var]
        fld   st0
        fld   st0
        fmulp
        fld1
        faddp
        fsqrt
        faddp
        fyl2x
        fldl2e
        fdiv




print:
        fst   qword [res]
        push  dword [res+4]
        push  dword [res]
        push  print_format
        call  printf
        add   esp, 16
        ret
