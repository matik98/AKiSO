START     STA BYTE
          LDX #7
BITSLP    LDA #'0'
          ASL BYTE
          BCC PR
          LDA #'1'
PR        STA ($80),Y
          INY
          DEX
          BPL BITSLP
          RTS
END
