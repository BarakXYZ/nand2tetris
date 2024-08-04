    //** Written & Designed by BarakXYZ (Barak Arosh Surian) in 2024
    // As part of Week 9 assignment @NAND2TETRIS by Shimon Shocken & Noam Nisan *//

    // Challenge LOLO (Part of Challenge 4)
    // RAM[LOLO] - Address 1010

    @1010  // @LOLO
    D=A+1  // 1010+1 -> D=1011
    @LOLI
    M=D-1  // 1011-1 RAM[LOLI] = 1010

    (LOLI)
    MD=M-D
           // First Run:  1010-1011 -> RAM[LOLI] = -1 | D = -1
           // Second Run: -1 -(-1) -> RAM[LOLI] = 0 | D = 0

    @LOLI  // @16
    D+1;JEQ  // Run1: -1 + 1 = 0;JEQ (TRUE) | Run2: 0 + 1 = 1;JEQ (FALSE)

    @1010  // @LOLO
    M=D  // RAM[1010] = 0

    // Answer: RAM[1010] = 0
