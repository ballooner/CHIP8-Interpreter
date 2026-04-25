#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// Initialize everything needed for the emulator and start it
// Provide the path of the ROM you want to execute
// Return 0 on success and -1 on failure
int initEmulator(char* romPath);
// Read the instructions from the romFile pointer into memory
// starting at INSTRUCTION_START.
// Return 0 on success and -1 on failure
int loadProgram(char* romPath);
// Load the font starting at FONT_START and ending at FONT_END
// Return 0 on success and -1 on failure
void loadFont(void);
// Fetch the next instruction and return it
uint16_t fetchInstruction(void);
// Decode and execute the provided instruction
int decodeAndExecute(SDL_Renderer *renderer, uint16_t instruction);
// Push a value to the stack
// Return true if successfull and false otherwise
int8_t stackPush(uint16_t value);
// Pop and return a value from the stack
int16_t stackPop(void);

#endif
