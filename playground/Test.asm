@R0
D=M
@N
M=D
@i
M=1
@SUM
M=0

(MAIN)
@N
D=M
@END
D;JEQ

@i
D=M
@SUM
M=M+D

@i
M=M+1
@N
M=M-1
@MAIN
0;JMP

(END)
@SUM
D=M
@R1
M=D

(INF)
@INF
0;JMP
