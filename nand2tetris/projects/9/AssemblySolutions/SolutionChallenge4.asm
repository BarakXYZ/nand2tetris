    //** Written & Designed by BarakXYZ (Barak Arosh Surian) in 2024
    // As part of Week 9 assignment @NAND2TETRIS by Shimon Shocken & Noam Nisan *//

    // Challenge 4 (SHIMON)
    
    // INIT Predefined Value
    @42
    D=A
    @SHIMON
    M=D
    @LOLO
    M=0
    @18
    D=A
    @GNIRUT
    M=D
    // RAM[SHIMON] = 42
    // RAM[LOLO] = 0
    // RAM[GNIRUT] = 18

    // Actual Challenge:
    @LOLO
    D=M  // D=0
    @GNIRUT  // @357
    D=D-M  // -18

    (FROGGY)
    @SHIMON
    D=M-D
        // Run 1: 42 - (-18) = 60
        // Run 2: 42 - 60 = -18

    @FROGGY
    D+1;JGE  // Run 1: 61 <= 0 (True) | Run 2: -17 <= 0 (False)

    @201
    M=A-D  // 201 -(-18) = 219 -> RAM[201] = 219

    // Answer: RAM[201] = 219
