#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "chip8.h"

#define WINDOW_WIDTH 	64
#define WINDOW_HEIGHT 	32
#define FRAME_RATE		"60"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argv[])
{
	if (!SDL_CreateWindowAndRenderer("Hello World", WINDOW_WIDTH, WINDOW_HEIGHT,
			   					    	0, &window, &renderer))
	{
		SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	// Set the rate SDL_AppIterate gets called to 60
	if (!SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, FRAME_RATE))
	{
		SDL_Log("Couldn't set callback rate: %s", SDL_GetError());
	   	return SDL_APP_FAILURE;
	}
	initEmulator(argv[1]);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
	if (event->type == SDL_EVENT_KEY_DOWN ||
			event->type == SDL_EVENT_QUIT)
	{
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
	SDL_FRect pixel = {10, 10, 10, 10};

	// Clear Screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	// Set color red and draw pixel
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderRect(renderer, &pixel);

	// Draw Frame
	SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
