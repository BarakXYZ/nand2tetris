    //** Written & Designed by BarakXYZ (Barak Arosh Surian) in 2024
    // As part of Week 9 assignment @NAND2TETRIS by Shimon Shocken & Noam Nisan *//

    // Challenge 2 (SHIMON)
    // INIT Predefined Value
    @42
    D=A
    @555
    M=D

    // Actual Challenge:
    @555  // @SHIMON
    D=M
    D=D+A  // D=555+42 -> D=597
    @40
    A=D-A  // @557 (597-40=557)
    M=D-1  // RAM[557]=596

    // RAM[555] = 42
    // RAM[557] = 596
