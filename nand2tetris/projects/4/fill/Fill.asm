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
@screen_address
M=D

@LOOP_WHITE  // if 0, white
D;JEQ

@LOOP_BLACK  // else, black
0;JMP

(LOOP_WHITE)
@KBD
D=M
@LOOP_WHITE  // if 0, white
D;JEQ

@LOOP_BLACK  // else, black
0;JMP

(LOOP_BLACK)
@KBD
D=M
@LOOP_WHITE  // if 0, white
D;JEQ

@LOOP_BLACK  // else, black
0;JMP
