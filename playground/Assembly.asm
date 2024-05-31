@R0
D=M
@temp
M=D

@R1
D=M
@R0
M=D

@temp
D=M
@R1
M=D





// Flush & init
@R1
M=0

@sum
M=0
@i
M=1

@R0
D=M
@n
M=D

(MAIN)
@n
D=M
@END
D;JEQ  // if (n == 0) END, else continue

@i
D=M
@sum  // sum+i
M=M+D

@i  // i++
M=M+1
@n
M=M-1  // n-1
@MAIN
0;JMP

// Store value at Register 1
(END)
@sum
D=M
@R1
M=D

// Loop4Ever
(INF)
@INF
0;JMP


