// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

// Flush variables
@mult_sum
M=0
@i
M=1

// if R0 == 0 || R1 == 0 -> return 0
@R0
D=M
@END
D;JEQ

@R1
D=M
@END
D;JEQ

// init
(LOOP)  // Main loop
@R0
D=M  // D=RAM[0]
@mult_sum
M=M+D  // mult_sum += RAM[0]

@R1
D=M  // D=RAM[1]
@i
D=M-D  // D=i-RAM[1]  (essentially, when i==RAM[i] -> stop)
@END
D;JEQ

@i  // i++
M=M+1
@LOOP  // Return to main loop
0;JMP

(END)  // Assign mult_sum to RAM[2]
@mult_sum
D=M
@R2
M=D

(POST)  // Infinite loop (idle)
@POST
0;JMP
