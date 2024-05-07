// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

@SCREEN
D=A
@screen_address
M=D  // Store screen address (essentially where we'll start manipulating bits)

@8192  // Entire Screen-Map range (the sum of pixels we'll manipulate)
D=A
@screen_map  // Non-volatile (an alias for assignment only)
M=D
@n  // Volatile (we'll offset and traverse through n directly)
M=D
@i  // We'll subtract i-n and check if == 0
M=1

@KBD  // Check keyboard input; pivot white if no-input, else black.
D=M
@WHITE_LOOP  // if 0 (no keyboard input), white screen
D;JEQ
@BLACK_LOOP  // else, black screen
0;JMP

(WHITE_LOOP)  // Make screen white (Main-Loop)
@screen_address
A=M
M=0  // Assigning 0 to reset the screen (to white)

@i  // 1, 2, 3...8191, 8192
D=M
@n  // screen_map alias
D=D-M  // -8191, -8190, -8189...-1, 0
@RESET_PARAMS_ENTER_IDLE_WHITE
D;JEQ  // if 0; stop loop and enter idle
// Else; move to the next pixel and assign 0
@screen_address
M=M+1
@i
M=M+1
@WHITE_LOOP  // Repeat loop until all pixels are white
0;JMP

(RESET_PARAMS_ENTER_IDLE_WHITE)
// Reset n (screen_map) & i (1)
@SCREEN
D=A
@screen_address
M=D
@screen_map
D=M
@n
M=D
@i
M=1

(WHITE_IDLE)
// Check keyboard (post-white screen)
@KBD
D=M
@WHITE_IDLE  // if 0 (no keyboard input); continue idle state
D;JEQ
@BLACK_LOOP  // else; start black screen loop
0;JMP

(BLACK_LOOP)  // Make screen black; main loop
@screen_address
A=M
M=-1  // Assigning -1 to make all 16-bits 1 (aka black)

@i  // 1, 2, 3...8191, 8192
D=M
@n  // screen_map alias
D=D-M  // -8191, -8190, -8189...-1, 0
@RESET_PARAMS_ENTER_IDLE_BLACK
D;JEQ  // if 0, jump to idle
// Else; move to the next pixel and assign -1
@screen_address
M=M+1
@i
M=M+1
@BLACK_LOOP
0;JMP

(RESET_PARAMS_ENTER_IDLE_BLACK)
@SCREEN
D=A
@screen_address
M=D
@screen_map
D=M
@n
M=D
@i
M=1

(BLACK_IDLE)
// Check KBD (post-black screen)
@KBD
D=M
@WHITE_LOOP  // if 0 (no keyboard input), white screen
D;JEQ
@BLACK_IDLE  // else; continue idle
0;JMP


// BarakXYZ 2024
