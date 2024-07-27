    // Challenge 2 (SHIMON)
    // init
    @42
    D=A
    @555
    M=D

    @555
    D=M
    D=D+A  // D=555+42 -> D=597
    @40
    A=D-A  // @557 (597-40=557)
    M=D-1  // RAM[557]=596

    // RAM[555] = 42
    // RAM[557] = 596
