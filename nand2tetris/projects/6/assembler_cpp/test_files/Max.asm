// Computes RAM[1] = 1+...+RAM[0]
// Usage: put a number in RAM[0]
@16
M=1
@17
M=0

@16
D=M
@0
D=D-M
@17
D;JGT

@16
D=M
@17
M=D+M
@16
M=M+1
@4
0;JMP

@17
D=M
@1
M=D
@21
0;JMP
