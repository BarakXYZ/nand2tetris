    //** Written & Designed by BarakXYZ (Barak Arosh Surian) in 2024
    // As part of Week 9 assignment @NAND2TETRIS by Shimon Shocken & Noam Nisan *//

    // Challenge 3 (SHIMON)

    // INIT Predefined Value
    @18
    D=A
    @357
    M=D  // RAM[357] = 18 (GNIRUT challenge result)
    
    // Actual Challenge:
    @357
    D=M  // D=18
    @666
    M=A+D  // 666+18 -> RAM[666]=684

    // Answer: RAM[666] = 684
