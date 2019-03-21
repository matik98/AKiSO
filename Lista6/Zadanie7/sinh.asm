
section .data
  message:
        db    "Wprowadz x", 10, 0
  scan_format:
        db    "%lf", 0
  print_format:
        db    "sinhx = %f", 10, 0

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
        fld   qword [var]
        fldl2e
        fmul
        fst   qword[res]

        fld1
        fld   st1
        fprem
        f2xm1
        faddp
        fscale

        fld   st0
        fld1
        fdivr
        fsub


        fld1
        fld1
        faddp
        fdivp

  print:
        fst   qword [res]
        push  dword [res+4]
        push  dword [res]
        push  print_format
        call  printf
        add   esp, 16
        ret
