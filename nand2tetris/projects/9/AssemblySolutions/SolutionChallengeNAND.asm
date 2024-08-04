    //** Written & Designed by BarakXYZ (Barak Arosh Surian) in 2024
    // As part of Week 9 assignment @NAND2TETRIS by Shimon Shocken & Noam Nisan *//

    // Challenge NAND (Part of Challenge 5)
    // Content:
    // SymName: NAND
    // Address: Can be determined later to help wherever needed
    // Value: RAM[10]

    @5  // 0101 -> 5
    D=A
    (NANI!)
    @10  // 1010 -> 10

    D=D&A  // (1.) 0
           // (2.) 1010 - 1111 -> 10

    MD=!D  // (1.) MD=-1
           // (2.) MD=-11
    @NANI!
    D+1;JGE  // (1.) D=0
             // (2.) D=-10
    @10
    M=D|A  // D = 0000 1010 (10) | 1111 0101 (-11) = 1111 1111 (-1)


    // RAM[10] = -1

