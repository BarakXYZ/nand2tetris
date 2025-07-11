# Project 0:

Test submission

# Project 1 -> Boolean Functions and Gate Logic

Introduction to HDL and 15 elementary logic gates implementation:

### .hdl

- Not, Not16,
- And, And16,
- Or, Or16, Or8Way, Xor
- Mux, Mux16, Mux4Way16, Mux8Way16,
- DMux, DMux4Way, DMux8Way,

# Project 2 -> Boolean Arithmetic and the ALU

Implement a family of adders, and most importantly - the ALU (Arithmetic Logical Unit):

### .hdl

- HalfAdder, FullAdder
- Add16, Inc16
- ALU

# Project 3 -> Memory

Building the computer's main memory unit (RAM):

### .hdl

> a:

- Bit, Register
- RAM8, RAM64
- PC (Program Counter)
  > b:
- RAM512, RAM4K, RAM16K

# Project 4 -> Machine Language

Writing low-level programs using the Hack machine language:

### .asm

- fill: Fill screen with black pixels on keyboard press, flush on release.
- mult: Based on repetitive addition, multiply 2 register's state, and store the results in a third one.

# Project 5 -> Computer Architecture

Assemble all previously built building blocks into a general-purpose 16-bit computer called Hack.
Build Hack Central Processing Unit (CPU).
Integrate the CPU with the RAM, creating a full-blown computer system capable of executing programs written in the Hack machine language.

### .hdl

- CPU: The Hack Central Processing unit (CPU).
- Memory: The complete address space of the Hack computer's memory, including RAM and memory-mapped I/O.
- Computer: The Hack computer, consisting of CPU, ROM and RAM.

# Project 6 -> Assembler

Build an assembler; we'll develop the capability of translating symbolic Hack programs into binary code that can be executed as-is on the Hack platform.

### .cpp

- HACK-Assembler-XYZ: My implementation of the Hack Assembler in C++ (I will probably improve it every so often)

# Project 7 -> Virtual Machine I: Stack Arithmetic

Build a basic VM translator, focusing on the implementation of the VM language's stack arithmetic and memory access commands.
Translate VM language into Hack assembly language.

### .cpp

- VMTranslator: Basic VM-to-Assembly translator handling arithmetic/logical commands and memory segments (local, argument, this, that, constant, static, pointer, temp).

# Project 8 -> Virtual Machine II: Program Control

Extend the basic VM translator into a full-scale VM translator.
Handle the VM language's branching and function calling commands.
Implement bootstrap code that initializes the VM.

### .cpp

- VMTranslator: Full VM translator with branching (label, goto, if-goto), function commands (function, call, return), and bootstrap code.

# Project 9 -> High-Level Language

Write a program/game in Jack - a simple, Java-like, object-based programming language.
Familiarize yourself with high-level programming and the Jack OS API.

### .jack

#### The Legend of the Hack Assembly:

The Legend of the Hack Assembly is a collection of increasingly demanding Assembly code challenges. Solve all queries, jump between different memory cells, and figure out the final answers.

# Project 10 -> Compiler I: Syntax Analysis

Build a syntax analyzer (parser) for the Jack language.
Parse Jack programs according to the Jack grammar, producing XML output that renders the program's structure.

### .cpp

- JackAnalyzer: Tokenizer and Parser that outputs XML representation of Jack program structure.

# Project 11 -> Compiler II: Code Generation

Extend the syntax analyzer into a full-scale Jack compiler.
Generate VM code from Jack source code.

### .cpp

- JackAnalyzer: Complete Jack-to-VM compiler with symbol table, code generation for expressions, statements, and subroutine calls.

# Project 12 -> Operating System

Implement the Jack OS - a collection of software services that extend the language capabilities.
Build the OS entirely in Jack.

### .jack

- Math: Implements multiplication, division, sqrt, and other mathematical operations.
- Memory: Handles heap management and memory allocation (alloc, deAlloc).
- Screen: Handles pixel-level and geometric shape drawing on screen.
- Output: Handles text output, character maps, and cursor management.
- Keyboard: Handles user keyboard input.
- String: Implements the String type and string manipulation methods.
- Array: Implements array allocation and disposal.
- Sys: Handles system initialization and program execution (init, halt, wait, error).

### ~~~~ Additional Notes ~~~~

# Build and Submission Guidelines

## Language Configuration

- **Important**: In the language configuration file, only mention "debug" - do NOT mention C++
- This has caused significant issues in the past

## Compression Requirements

- Use only regular Windows/MacOS native zipping options
- When creating the zip file, compress only the **content** of the folder, not the folder itself
- When the zip is opened, files should be immediately visible (no nested folder structure)

## Build System

- Add a Makefile to the project
- Reference examples from Project 7 & 8 for implementation guidance
