// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

// To access keyboard
// @KBD
// D=A
// @key_addr
// M=D
// @key_addr
// A=M
// D=M
// It's not really needed for the keyboard though, more for the screen

@SCREEN
D=A
@screen_address  // 16,384
M=D  // Store screen address

@1
D=A
@offset
M=D

@32
D=A
@row
M=D

@256
D=A
@n
M=D  // Store number of rows (256)

@i
M=1  // Initialize i to 1

@KBD
D=M
@WHITE_LOOP  // if 0 (no keyboard input), white screen
D;JEQ

@BLACK_LOOP  // else, black screen
0;JMP

(WHITE_LOOP)  // Make screen white; main body
// body
@screen_address
A=M
M=0  // Make screen row white

@i  // 1
D=M
@n  // 256
D=D-M  // -511
@RESET_PARAMS_ENTER_IDLE_WHITE
D;JEQ  // if 0, jump to idle
// Else; increase address by 64
@offset
D=M
@screen_address
M=D+M
@i
M=M+1
@WHITE_LOOP
0;JMP

(RESET_PARAMS_ENTER_IDLE_WHITE)
// Reset n (256) & i (1)
@SCREEN
D=A
@screen_address
M=D
@256
D=A
@n
M=D
@i
M=1

(WHITE_IDLE)
// Check KBD (post-white screen)
@KBD
D=M
@WHITE_IDLE  // if 0 (no keyboard input), white screen
D;JEQ

@BLACK_LOOP  // else, black screen
0;JMP

(BLACK_LOOP)  // Make screen white; main body
// body
@screen_address
A=M
M=-1  // Make screen row white

@i  // 1
D=M
@n  // 256
D=D-M  // -511
@RESET_PARAMS_ENTER_IDLE_BLACK
D;JEQ  // if 0, jump to idle
// Else; increase address by 64
@offset
D=M
@screen_address
M=D+M
@i
M=M+1
@BLACK_LOOP
0;JMP

(RESET_PARAMS_ENTER_IDLE_BLACK)
// Reset n (256) & i (1)
@SCREEN
D=A
@screen_address
M=D
@256
D=A
@n
M=D
@i
M=1

(BLACK_IDLE)
// Check KBD (post-white screen)
@KBD
D=M
@WHITE_LOOP  // if 0 (no keyboard input), white screen
D;JEQ

@BLACK_IDLE
0;JMP
