// push constant 17
@17
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 17
@17
D=A
@SP
AM=M+1
A=A-1
M=D
// eq
@SP
AM=M-1
D=M
A=A-1
D=M-D
@EQUAL_0
D;JEQ
D=0
@NOT_EQUAL_0
0;JMP
(EQUAL_0)
D=-1
(NOT_EQUAL_0)
@SP
A=M-1
M=D
// push constant 17
@17
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 16
@16
D=A
@SP
AM=M+1
A=A-1
M=D
// eq
@SP
AM=M-1
D=M
A=A-1
D=M-D
@EQUAL_1
D;JEQ
D=0
@NOT_EQUAL_1
0;JMP
(EQUAL_1)
D=-1
(NOT_EQUAL_1)
@SP
A=M-1
M=D
// push constant 16
@16
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 17
@17
D=A
@SP
AM=M+1
A=A-1
M=D
// eq
@SP
AM=M-1
D=M
A=A-1
D=M-D
@EQUAL_2
D;JEQ
D=0
@NOT_EQUAL_2
0;JMP
(EQUAL_2)
D=-1
(NOT_EQUAL_2)
@SP
A=M-1
M=D
// push constant 892
@892
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 891
@891
D=A
@SP
AM=M+1
A=A-1
M=D
// lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LESS_THAN_0
D;JLT
D=0
@NOT_LESS_THAN_0
0;JMP
(LESS_THAN_0)
D=-1
(NOT_LESS_THAN_0)
@SP
A=M-1
M=D
// push constant 891
@891
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 892
@892
D=A
@SP
AM=M+1
A=A-1
M=D
// lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LESS_THAN_1
D;JLT
D=0
@NOT_LESS_THAN_1
0;JMP
(LESS_THAN_1)
D=-1
(NOT_LESS_THAN_1)
@SP
A=M-1
M=D
// push constant 891
@891
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 891
@891
D=A
@SP
AM=M+1
A=A-1
M=D
// lt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@LESS_THAN_2
D;JLT
D=0
@NOT_LESS_THAN_2
0;JMP
(LESS_THAN_2)
D=-1
(NOT_LESS_THAN_2)
@SP
A=M-1
M=D
// push constant 32767
@32767
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 32766
@32766
D=A
@SP
AM=M+1
A=A-1
M=D
// gt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@GREATER_0
D;JGT
D=0
@NOT_GREATER_0
0;JMP
(GREATER_0)
D=-1
(NOT_GREATER_0)
@SP
A=M-1
M=D
// push constant 32766
@32766
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 32767
@32767
D=A
@SP
AM=M+1
A=A-1
M=D
// gt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@GREATER_1
D;JGT
D=0
@NOT_GREATER_1
0;JMP
(GREATER_1)
D=-1
(NOT_GREATER_1)
@SP
A=M-1
M=D
// push constant 32766
@32766
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 32766
@32766
D=A
@SP
AM=M+1
A=A-1
M=D
// gt
@SP
AM=M-1
D=M
A=A-1
D=M-D
@GREATER_2
D;JGT
D=0
@NOT_GREATER_2
0;JMP
(GREATER_2)
D=-1
(NOT_GREATER_2)
@SP
A=M-1
M=D
// push constant 57
@57
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 31
@31
D=A
@SP
AM=M+1
A=A-1
M=D
// push constant 53
@53
D=A
@SP
AM=M+1
A=A-1
M=D
// add
@SP
AM=M-1
D=M
A=A-1
M=M+D
// push constant 112
@112
D=A
@SP
AM=M+1
A=A-1
M=D
// sub
@SP
AM=M-1
D=M
A=A-1
M=M-D
// neg
@SP
A=M-1
M=-M
// and
@SP
AM=M-1
D=M
A=A-1
M=D&M
// push constant 82
@82
D=A
@SP
AM=M+1
A=A-1
M=D
// or
@SP
AM=M-1
D=M
A=A-1
M=D|M
// not
@SP
A=M-1
M=!M
