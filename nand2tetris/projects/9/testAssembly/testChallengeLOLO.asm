    // Challenge LOLO (Part of Challenge 4)

    @1010  // init
    D=A+1  // 1010+1 -> D=1011
    @LOLI  // @16
    M=D-1  // 1011-1 RAM[LOLI] = 1010

    (LOLI)
    MD=M-D
           // First Run:  1010-1011 -> RAM[LOLI] = -1 | D = -1
           // Second Run: -1 -(-1) -> RAM[LOLI] = 0 | D = 0

    @LOLI  // @16
    D+1;JEQ  // Run1: -1 + 1 = 0;JEQ (TRUE) | Run2: 0 + 1 = 1;JEQ (FALSE)

    @1010
    M=D  // RAM[1010] = 0

    // Answer: RAM[1010] = 0
