    @R0
    D=M
    @N
    M=D

    @SUM
    M=0
    
    (BODY)
    @END
    D;JEQ
    @SUM
    M=M+D
    @N
    MD=M-1
    @BODY
    0;JMP

    (END)
    @SUM
    D=M
    @R1
    M=D

    (INF)
    @INF
    0;JMP
