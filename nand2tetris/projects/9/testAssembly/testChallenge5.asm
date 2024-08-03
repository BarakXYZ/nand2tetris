    // Challenge 5 (SHIMON)

    // INIT

    // RAM[NAND]=-1
    @NAND
    M=-1

    // RAM[TETRIS]=-476
    @475
    D=!A
    @TETRIS
    M=D


    @NAND    // 0. @
    D=M      // 1. D=-1
    @2
    D=D+A    // D=1
    @TETRIS
    D=D-M    // D=1-(-476)=477
    
    @WEEK9
    M=!D     // RAM[WEEK9]=-478

    @ThankU
    M=1      // RAM[ThankU]=1

