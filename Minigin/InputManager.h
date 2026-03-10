#pragma once
#include "Singleton.h"
#include "InputState.h"
#include "Command.h"
#include <memory>
#include <vector>
#include <SDL3/SDL.h>

namespace dae
{
	class Controller;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		
		void BindKeyboardCommand(SDL_Scancode key, InputState state, std::unique_ptr<Command> command);
		void BindControllerCommand(int button, InputState state, std::unique_ptr<Command> command, int controllerIndex = 0);

		void UnbindKeyboardCommand(SDL_Scancode key, InputState state);
		void UnbindControllerCommand(int button, InputState state, int controllerIndex = 0);

	private:
		friend class Singleton<InputManager>;
		InputManager() = default;

		struct KeyboardBinding
		{
			SDL_Scancode key;
			InputState state;
			std::unique_ptr<Command> command;
		};

		struct ControllerBinding 
		{
			int button;
			InputState state;
			std::unique_ptr<Command> command;
			int controllerIndex;

		};

		std::vector<KeyboardBinding> m_KeyboardBindings{};
		std::vector<ControllerBinding> m_ControllerBindings{};
		std::vector<std::unique_ptr<Controller>> m_Controllers{};

	};

}
