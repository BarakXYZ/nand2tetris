// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/6/max/Max.asm

// Computes R2 = max(R0, R1)  (R0,R1,R2 refer to RAM[0],RAM[1],RAM[2])
// Usage: Before executing, put two values in R0 and R1.

  // D = R0 - R1
  @R0
  D=M
  @R1
  D=D-M
  // If (D > 0) goto ITSR0
  @ITSR0
  D;JGT
  // Its R1
  @R1
  D=M
  @OUTPUT_D
  0;JMP

  // My test
  @sum
  M=1
  D=M
  // My test


(ITSR0)  // Associated with line 10
  @R0
  D=M
(OUTPUT_D)
  @R2
  M=D

  // My test
  @sum
  M=0
  D=M+D
  // My test

(END)
  @END
  0;JMP
