    // Challenge TETRIS (Part of Challenge 5)

    // SymName = TETRIS
    // Address = 475 (ASCII value of TETRIS haha idk:()
    // Value = ?
    // RAM[101]=292
    // RAM[324]=198
    // RAM[0]=130
    // RAM[487]=12
    // RAM[367]=10

    // RAM[SHIMON]=42, Address: 555
    // RAM[GNIRUT]=18, Address: 357
    // RAM[LOLO]=0,    Address: 1010
    // RAM[NAND]=-1,   Address: 289
    // INIT

    // RAM[SHIMON]
    @42
    D=A
    @555
    M=D

    // RAM[GNIRUT]
    @18
    D=A
    @357
    M=D

    // RAM[LOLO]
    @1010
    M=0

    // RAM[NAND]
    @289
    M=-1

    // RAM[101]=292 Used
    @292
    D=A
    @101
    M=D

    // RAM[375]=198 Used
    @198
    D=A
    @375
    M=D

    // RAM[367]=10 Used
    @10
    D=A
    @367
    M=D
    
    // RAM[0]=130 Used
    @130
    D=A
    @0
    M=D
    
    // RAM[487] Used
    @12
    D=A
    @487
    M=D

    D=0
    

    @101       // 0. Note: RAM[101]=292
    D=A        // 1. D=101
    AD=D|M     // 2. 101|292 = @357 (@GNIRUT), D=357
    A=M|D      // 3. 18|357 = @375
    AM=M+D     // 4. 357 + 198 = @555 (@SHIMON), RAM[375]=555
    AD=M|D     // 5. 42|357 = @367, D=367
    AMD=D-M    // 6. 367-10 = @357 (@GNIRUT), RAM[367]=357, D=357
    A=D&M      // 7. 357 & 18 = 32 -> @0
    AMD=D|M    // 8. 357|180 = @487, RAM[0]=487, D=487
    A=D-M      // 9. 487-12 = @475
    M=!A       // 10. RAM[475]=-476

    // Answers: 
    // 1. RAM[375]=555
    // 2. RAM[367]=357
    // 3. RAM[0]=487
    // 4. RAM[475]=-476


    // Hints:
    // RAM[101]=292
    // RAM[324]=198
    // RAM[0]=130
    // RAM[487]=12
    // RAM[367]=10

