    //** Written & Designed by BarakXYZ (Barak Arosh Surian) in 2024
    // As part of Week 9 assignment @NAND2TETRIS by Shimon Shocken & Noam Nisan *//

    // Challenge 5 (SHIMON)

    // INIT Predefined Value
    // RAM[NAND]=-1
    @NAND
    M=-1

    // RAM[TETRIS]=-476
    @475
    D=!A
    @TETRIS
    M=D


    // Actual Challenge:
    @NAND    // 0. @289
    D=M      // 1. D=-1
    @2
    MD=D+A    // MD=1
    @TETRIS  // @475
    D=D-M    // D=1-(-476)=477
    
    @WEEK9
    M=!D     // RAM[WEEK9]=-478

    // Answers:
    // 1. RAM[NAND]=-1
    // 2. RAM[2]= 1
    // 3. RAM[TETRIS]=-476
    // 4. RAM[WEEK9]=-478
