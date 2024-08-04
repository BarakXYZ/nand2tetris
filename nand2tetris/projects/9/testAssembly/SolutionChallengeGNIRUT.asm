    //** Written & Designed by BarakXYZ (Barak Arosh Surian) in 2024
    // As part of Week 9 assignment @NAND2TETRIS by Shimon Shocken & Noam Nisan *//

    // Challenge GNIRUT (Part of Challenge 3)

    // INIT Predefined Value
    @5
    D=A
    @3
    M=D
    D=A
    @7
    M=D
    D=A
    @5
    M=D
    // RAM[3] = 5
    // RAM[5] = 7
    // RAM[7] = 3


    // Actual Challenge:
    @5
    D=A    // D=5
    D=D+M  // 5+7 -> D=12
    A=D-A  // 12-5 -> @7
    D=D+1  // 12+1 -> D=13

    D=D-M  // 13-3 -> D=10
    A=D-A  // 10-7 -> @3

    D=M+D  // 5+10 -> D=15
    D=A+D  // 3+15 -> D=18

    @357
    M=D    // RAM[357]=18

    // Answer: RAM[357]=18
