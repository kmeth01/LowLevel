## Gamul8 : 
Refer : https://en.wikipedia.org/wiki/CHIP-8 


# – Source and Header files:
  emul8.cpp
  gamul.cpp
  gamul.h
  beep.aiff

# – General Overview :
  Gamul.h describes the implementation of class gamul8.

 
# Init Stage :
  1. Memory, Stack, Keys, Index , Registers, Processor are initialized.
  2. PC counter is intialized to 512. 
  3. Fontset is loaded to memory.
  4. Application size is calculated to check if it fits in memory. if it doesnt fit, execution doesnt proceed any further.
  5. Application is loaded to memory. 
  
# Execution Cycle : 
  1. Fetch Stage : Opcode at PC is read (2 bytes).
  2. opcode can start with 16 (0-F) different ways. Processor vector has 16 function pointers which can be dereferenced using I in IXXX (opcode).
  3. delay timer is decreased if there is any.
  4. soundtimer is decreased if there is any.


# Compile using:

g++ emul8.cpp gamul.cpp -o emulator -lGL -lGLU -lglut -Wall

Example Run command:

./emulator ./games/BLINKY

