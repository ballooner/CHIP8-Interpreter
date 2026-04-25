#include "chip8.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// CHIP8 DEFINES
#define FONT_START 			0x050
#define FONT_END			0x09F
#define INSTRUCTION_START	0x200
#define INSTRUCTION_RATE	700		// Execute x instructions per second
#define INSTRUCTION_TIME	((double)(INSTRUCTION_RATE / 1000))

// CHIP8 BITMASKS
#define OPCODE_BITMASK		0xF000
#define X_BITMASK			0x0F00
#define Y_BITMASK			0x00F0
#define N_BITMASK			0x000F
#define NN_BITMASK			0x00FF
#define NNN_BITMASK			0x0FFF

// CHIP8 VARIABLES
static int8_t memory[4096];
static int16_t stack[16];
uint8_t stackPointer = 0;
uint16_t indexRegister;
int8_t variableRegisters[16];
uint16_t pc = INSTRUCTION_START;
uint8_t delayTimer;
uint8_t soundTimer;

// Initialize everything needed for the emulator
int initEmulator(char* const romPath)
{
	loadFont();
	if (loadProgram(romPath)) return -1;

	return 0;
}

int loadProgram(char* const romPath)
{
	FILE* rom;
	if ((rom = fopen(romPath, "r")) == NULL) return -1;

	uint16_t i = 0;
	while (fread(memory + INSTRUCTION_START + i++, 1, 1, rom) == 1);
	fclose(rom);

	return 0;
}

// Load the font starting at FONT_START and ending at FONT_END
void loadFont(void)
{
	uint8_t font_array[] = 
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	uint8_t i = FONT_START;
	while (i <= FONT_END && i <= (sizeof(font_array) / sizeof(font_array[0])))
	{
		memory[i] = font_array[i - FONT_START];
		i++;
	}
}

// Fetch the next instruction and return it
uint16_t fetchInstruction(void)
{
	uint16_t instruction = memory[pc++] << 4;
	instruction += memory[pc++];

	return instruction;
}

// Decode and execute the provided instruction
int decodeAndExecute(SDL_Renderer *renderer, uint16_t instruction)
{
	uint16_t opcode = instruction & OPCODE_BITMASK;
	int8_t xReg;
	int8_t yReg;

	switch (opcode)
	{
	case (0x0000):
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		break;
	case (0x1000):
		pc = instruction & NNN_BITMASK;
		break;
	case (0x6000):
		variableRegisters[(instruction & X_BITMASK) >> 8] 
			= instruction & NN_BITMASK;
		break;
	case (0x7000):
		variableRegisters[(instruction & X_BITMASK) >> 8]
			+= instruction & NN_BITMASK;
		break;
	case (0xA000):
		indexRegister = instruction & NNN_BITMASK;
		break;
	case (0xD000):
		xReg = variableRegisters[(instruction & X_BITMASK) >> 8] % 64;
		yReg = variableRegisters[(instruction & Y_BITMASK) >> 4] % 32;

		break;
	default:
		printf("Instruction not found\n");
		return 1;
	}

	return 0;
}

// Push a value to the stack
// Return 0 if successful and -1 otherwise
int8_t stackPush(uint16_t value)
{
	if (stackPointer < 15)
	{
		stack[stackPointer++] = value;
		return 0;
	}
	
	return -1;
}

// Pop and return a value from the stack
int16_t stackPop(void)
{
	if (stackPointer > 0)
	{
		return stack[stackPointer--];
	}

	return stackPointer;
}
