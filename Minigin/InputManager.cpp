#include <SDL3/SDL.h>
#include "InputManager.h"

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))  //SDL_PollEvent returns true if there is an event in the queue, 
		//false otherwise, so we can use it in a while loop to process all events until the queue is empty
	{
		if (e.type == SDL_EVENT_QUIT) //event that is trigger when the user clicks the close button(x) of the window
		{
			return false;
		}
		if (e.type == SDL_EVENT_KEY_DOWN) //event that is trigger when the user presses a key on the keyboard
		{
			
		}
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) 
		{
			
		}
		// etc...
	}

	return true;
}
