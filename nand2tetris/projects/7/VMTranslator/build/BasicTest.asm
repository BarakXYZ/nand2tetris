// push constant 10
@10
D=A
@SP
A=M
// SP++
M=D
M=M+1
// pop local 0
// push constant 21
@21
D=A
@SP
A=M
// SP++
M=D
M=M+1
// push constant 22
@22
D=A
@SP
A=M
// SP++
M=D
M=M+1
// pop argument 2
// pop argument 1
// push constant 36
@36
D=A
@SP
A=M
// SP++
M=D
M=M+1
// pop this 6
// push constant 42
@42
D=A
@SP
A=M
// SP++
M=D
M=M+1
// push constant 45
@45
D=A
@SP
A=M
// SP++
M=D
M=M+1
// pop that 5
// pop that 2
// push constant 510
@510
D=A
@SP
A=M
// SP++
M=D
M=M+1
// pop temp 6
// push local 0
@LCL
D=A
@0D=A+D
@13
M=D
// *SP = *addr
A=M
D=M
@SP
A=M
M=D
// SP++
@SP
M=M+1
// push that 5
@THAT
D=A
@5D=A+D
@13
M=D
// *SP = *addr
A=M
D=M
@SP
A=M
M=D
// SP++
@SP
M=M+1
// add
// push argument 1
@ARG
D=A
@1D=A+D
@13
M=D
// *SP = *addr
A=M
D=M
@SP
A=M
M=D
// SP++
@SP
M=M+1
// sub
// push this 6
@THIS
D=A
@6D=A+D
@13
M=D
// *SP = *addr
A=M
D=M
@SP
A=M
M=D
// SP++
@SP
M=M+1
// push this 6
@THIS
D=A
@6D=A+D
@13
M=D
// *SP = *addr
A=M
D=M
@SP
A=M
M=D
// SP++
@SP
M=M+1
// add
// sub
// push temp 6
// add
