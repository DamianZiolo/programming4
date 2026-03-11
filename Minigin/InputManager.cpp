#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include "Controller.h"

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
		// etc...
		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	//it returns array of bools
	const bool* keyboardState = SDL_GetKeyboardState(nullptr);

	//Check and execute all valid keyboard bindings
	for (auto& binding : m_KeyboardBindings)
	{
		bool execute = false;

		switch (binding.state)
		{
		case InputState::Pressed:
			execute = keyboardState[binding.key];
			break;

		// 0 0 - nothing
		// 0 1 - down
		// 1 1 - pressed
		// 1 0 - up

		case InputState::Down:
			execute = !m_PreviousKeyboardState[binding.key] && keyboardState[binding.key];
			break;

		case InputState::Up:
			execute = m_PreviousKeyboardState[binding.key] && !keyboardState[binding.key];
			break;

		}

		if (execute)
		{
			binding.command->Execute();
		}
	}

	m_PreviousKeyboardState.assign(
		keyboardState,
		keyboardState + SDL_SCANCODE_COUNT
	);

	
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}
	//Check and execute all valid controllers bindings
	for (auto& binding : m_ControllerBindings)
	{
		bool execute = false;

		//Check if controler is valid
		if (binding.controllerIndex >= m_Controllers.size())
		{
			continue;
		}
		
		auto& controller = m_Controllers[binding.controllerIndex];

		switch (binding.state)
		{
		case InputState::Pressed:
			execute = controller->IsPressed(binding.button);
			break;

		case InputState::Down:
			execute = controller->IsDownThisFrame(binding.button);
			break;

		case InputState::Up:
			execute = controller->IsUpThisFrame(binding.button);
			break;
		}

		if (execute)
		{
			binding.command->Execute();
		}
	}


	return true;
}

void dae::InputManager::BindKeyboardCommand(SDL_Scancode key, InputState state, std::unique_ptr<Command> command)
{
	assert(command && "BindKeyboardCommand received nullptr command");

	if (key == SDL_SCANCODE_UNKNOWN || state == InputState::None)
	{
		return;
	}
		
	//we need move command because it's unique pointer
	m_KeyboardBindings.emplace_back(KeyboardBinding{ key,state,std::move(command) });
}

void dae::InputManager::BindControllerCommand(int button, InputState state, std::unique_ptr<Command> command, int controllerIndex)
{
	assert(command && "BindControllerCommand received nullptr command");
	if (button < 0 || state == InputState::None || controllerIndex < 0)
	{
		return;
	}

	//we need move command because it's unique pointer
	m_ControllerBindings.emplace_back(ControllerBinding{ button,state,std::move(command),controllerIndex });
}

void dae::InputManager::UnbindKeyboardCommand(SDL_Scancode key, InputState state)
{
	if(key == SDL_SCANCODE_UNKNOWN || state == InputState::None)
	{
		return;
	}

	for (auto iterator = m_KeyboardBindings.begin(); iterator != m_KeyboardBindings.end(); ++iterator)
	{
		if(iterator->key == key && iterator->state == state)
		{
			m_KeyboardBindings.erase(iterator);
			return;
		}

	}

}

void dae::InputManager::UnbindControllerCommand(int button, InputState state, int controllerIndex)
{
	if (button < 0 || state == InputState::None || controllerIndex < 0)
	{
		return;
	}

	for (auto iterator = m_ControllerBindings.begin(); iterator != m_ControllerBindings.end(); ++iterator)
	{
		if (iterator->button == button && iterator->state == state && iterator->controllerIndex == controllerIndex)
		{
			m_ControllerBindings.erase(iterator);
			return;
		}
	}

}
