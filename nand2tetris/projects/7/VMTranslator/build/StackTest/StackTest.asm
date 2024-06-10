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
@EQUAL1
D;JEQ
D=0
@NOT_EQUAL1
0;JMP
(EQUAL1)
D=-1
(NOT_EQUAL1)
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
@EQUAL2
D;JEQ
D=0
@NOT_EQUAL2
0;JMP
(EQUAL2)
D=-1
(NOT_EQUAL2)
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
@EQUAL3
D;JEQ
D=0
@NOT_EQUAL3
0;JMP
(EQUAL)
D=-1
(NOT_EQUAL3)
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
@LESS_THAN1
D;JLT
D=0
@NOT_LESS_THAN1
0;JMP
(LESS_THAN1)
D=-1
(NOT_LESS_THAN1)
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
@LESS_THAN2
D;JLT
D=0
@NOT_LESS_THAN2
0;JMP
(LESS_THAN2)
D=-1
(NOT_LESS_THAN2)
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
@LESS_THAN3
D;JLT
D=0
@NOT_LESS_THAN3
0;JMP
(LESS_THAN3)
D=-1
(NOT_LESS_THAN3)
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
@GREATER1
D;JGT
D=0
@NOT_GREATER1
0;JMP
(GREATER1)
D=-1
(NOT_GREATER1)
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
@GREATER2
D;JGT
D=0
@NOT_GREATER2
0;JMP
(GREATER2)
D=-1
(NOT_GREATER2)
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
@GREATER3
D;JGT
D=0
@NOT_GREATER3
0;JMP
(GREATER3)
D=-1
(NOT_GREATER3)
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
