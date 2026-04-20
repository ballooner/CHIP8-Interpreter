#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// CHIP8 DEFINES
#define FONT_START 			0x050
#define FONT_END			0x09F
#define INSTRUCTION_START	0x200

// SDL DEFINES
#define WINDOW_WIDTH 	512
#define WINDOW_HEIGHT 	256
#define PIXEL_WIDTH 	(WINDOW_WIDTH / 64)
#define PIXEL_HEIGHT	(WINDOW_HEIGHT / 64)

// CHIP8 VARIABLES
static int8_t memory[4096];
static int16_t stack[16];
static uint8_t display[64][32];
uint8_t stackPointer = 0;

// SDL VARIABLES
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

// Initialize everything needed for the emulator and start it
void initEmulator(void)
{
	load_font();

}

// Load the font starting at FONT_START and ending at FONT_END
void load_font(void)
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
	while (i <= FONT_END)
	{
		memory[i] = font_array[i - FONT_START];
		i++;
	}
}

// Push a value to the stack
// Return true if successfull and false otherwise
int8_t stackPush(uint16_t value)
{
	if (stackPointer < 15)
	{
		stack[stackPointer++] = value;
		return true;
	}
	
	return false;
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

// SDL FUNCTIONS
