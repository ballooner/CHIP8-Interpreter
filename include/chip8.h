#ifndef CHIP8
#def CHIP8

#include <stdint.h>

#define FONT_START			0x050
#define FONT_END 			0x09F
#define INSTRUCTION_START	0x200

// Initialize everything needed for the emulator and start it
void initEmulator(void);
// Load the font starting at FONT_START and ending at FONT_END
void load_font(void);
// Push a value to the stack
// Return true if successfull and false otherwise
int8_t stackPush(uint16_t value);
// Pop and return a value from the stack
int16_t stackPop(void);

#endif
