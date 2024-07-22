    @42
    D=A
    @SHIMON
    M=D
    @0
    D=A
    @LOLO
    M=D
    @21
    D=A
    @GNIRUT
    M=D
    D=0

    @LOLO
    D=M
    @GNIRUT
    D=D-M  // -21
    (FROGGY)
    @SHIMON
    D=M-D  // 42 - (-21) = 63  | 42 - 63 = -21
    @FROGGY
    D+1;JGE  // 20 (greater) | -1 <= 0 (NO)
    @201
    M=A-D  // RAM[101] = 101 + 21 = 222
