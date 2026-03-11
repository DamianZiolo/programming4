#pragma once
#include "Singleton.h"
#include "InputState.h"
#include "Command.h"
#include <memory>
#include <vector>
#include <SDL3/SDL.h>
#include "Controller.h"
#include "ControllerButton.h"

namespace dae
{

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		
		void BindKeyboardCommand(SDL_Scancode key, InputState state, std::unique_ptr<Command> command);
		void BindControllerCommand(ControllerButton button, InputState state, std::unique_ptr<Command> command, int controllerIndex = 0);

		void UnbindKeyboardCommand(SDL_Scancode key, InputState state);
		void UnbindControllerCommand(ControllerButton button, InputState state, int controllerIndex = 0);

	private:
		friend class Singleton<InputManager>;
		InputManager();

		struct KeyboardBinding
		{
			SDL_Scancode key;
			InputState state;
			std::unique_ptr<Command> command;
		};

		struct ControllerBinding
		{
			ControllerButton button;
			InputState state;
			std::unique_ptr<Command> command;
			int controllerIndex;
		};

		std::vector<KeyboardBinding> m_KeyboardBindings{};
		std::vector<ControllerBinding> m_ControllerBindings{};
		std::vector<std::unique_ptr<Controller>> m_Controllers{};

		
		std::vector<uint8_t> m_PreviousKeyboardState{};
	};

}
