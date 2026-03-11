#include "Controller.h"

#if defined(__EMSCRIPTEN__)
#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#else
#include <Windows.h>
#include <Xinput.h>
#endif

#include <array>
#include <memory>

namespace
{
#if defined(__EMSCRIPTEN__)
	SDL_GamepadButton ToSDLButton(dae::ControllerButton button)
	{
		switch (button)
		{
		case dae::ControllerButton::DPadUp:    return SDL_GAMEPAD_BUTTON_DPAD_UP;
		case dae::ControllerButton::DPadDown:  return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
		case dae::ControllerButton::DPadLeft:  return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
		case dae::ControllerButton::DPadRight: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
		case dae::ControllerButton::A:         return SDL_GAMEPAD_BUTTON_SOUTH;
		case dae::ControllerButton::B:         return SDL_GAMEPAD_BUTTON_EAST;
		case dae::ControllerButton::X:         return SDL_GAMEPAD_BUTTON_WEST;
		case dae::ControllerButton::Y:         return SDL_GAMEPAD_BUTTON_NORTH;
		default:                              return SDL_GAMEPAD_BUTTON_INVALID;
		}
	}
#else
	WORD ToXInputButton(dae::ControllerButton button)
	{
		switch (button)
		{
		case dae::ControllerButton::DPadUp:    return XINPUT_GAMEPAD_DPAD_UP;
		case dae::ControllerButton::DPadDown:  return XINPUT_GAMEPAD_DPAD_DOWN;
		case dae::ControllerButton::DPadLeft:  return XINPUT_GAMEPAD_DPAD_LEFT;
		case dae::ControllerButton::DPadRight: return XINPUT_GAMEPAD_DPAD_RIGHT;
		case dae::ControllerButton::A:         return XINPUT_GAMEPAD_A;
		case dae::ControllerButton::B:         return XINPUT_GAMEPAD_B;
		case dae::ControllerButton::X:         return XINPUT_GAMEPAD_X;
		case dae::ControllerButton::Y:         return XINPUT_GAMEPAD_Y;
		default:                               return 0;
		}
	}
#endif
}

namespace dae
{
	class Controller::Impl
	{
	public:
		explicit Impl(int controllerIndex)
			: m_ControllerIndex(controllerIndex)
		{
		}

#if defined(__EMSCRIPTEN__)
		~Impl()
		{
			if (m_pGamepad)
			{
				SDL_CloseGamepad(m_pGamepad);
				m_pGamepad = nullptr;
			}
		}
#endif

		void Update()
		{
#if defined(__EMSCRIPTEN__)
			for (int i = 0; i < static_cast<int>(SDL_GAMEPAD_BUTTON_COUNT); ++i)
			{
				m_PreviousButtons[i] = m_CurrentButtons[i];
			}

			OpenGamepadIfNeeded();

			if (!m_pGamepad)
			{
				for (int i = 0; i < static_cast<int>(SDL_GAMEPAD_BUTTON_COUNT); ++i)
				{
					m_CurrentButtons[i] = false;
				}
				m_IsConnected = false;
				return;
			}

			m_IsConnected = true;

			for (int i = 0; i < static_cast<int>(SDL_GAMEPAD_BUTTON_COUNT); ++i)
			{
				m_CurrentButtons[i] = SDL_GetGamepadButton(
					m_pGamepad,
					static_cast<SDL_GamepadButton>(i)
				);
			}
#else
			m_PreviousState = m_CurrentState;
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

			const DWORD result = XInputGetState(m_ControllerIndex, &m_CurrentState);
			m_IsConnected = (result == ERROR_SUCCESS);

			if (!m_IsConnected)
			{
				ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
				m_ButtonsPressedThisFrame = 0;
				m_ButtonsReleasedThisFrame = 0;
				return;
			}

			const auto buttonChanges =
				m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;

			m_ButtonsPressedThisFrame =
				buttonChanges & m_CurrentState.Gamepad.wButtons;

			m_ButtonsReleasedThisFrame =
				buttonChanges & (~m_CurrentState.Gamepad.wButtons);
#endif
		}

		bool IsPressed(ControllerButton button) const
		{
#if defined(__EMSCRIPTEN__)
			const auto sdlButton = ToSDLButton(button);
			if (sdlButton == SDL_GAMEPAD_BUTTON_INVALID)
				return false;

			return m_CurrentButtons[static_cast<int>(sdlButton)];
#else
			const WORD xButton = ToXInputButton(button);
			return (m_CurrentState.Gamepad.wButtons & xButton) != 0;
#endif
		}

		bool IsDownThisFrame(ControllerButton button) const
		{
#if defined(__EMSCRIPTEN__)
			const auto sdlButton = ToSDLButton(button);
			if (sdlButton == SDL_GAMEPAD_BUTTON_INVALID)
				return false;

			const int index = static_cast<int>(sdlButton);
			return !m_PreviousButtons[index] && m_CurrentButtons[index];
#else
			const WORD xButton = ToXInputButton(button);
			return (m_ButtonsPressedThisFrame & xButton) != 0;
#endif
		}

		bool IsUpThisFrame(ControllerButton button) const
		{
#if defined(__EMSCRIPTEN__)
			const auto sdlButton = ToSDLButton(button);
			if (sdlButton == SDL_GAMEPAD_BUTTON_INVALID)
				return false;

			const int index = static_cast<int>(sdlButton);
			return m_PreviousButtons[index] && !m_CurrentButtons[index];
#else
			const WORD xButton = ToXInputButton(button);
			return (m_ButtonsReleasedThisFrame & xButton) != 0;
#endif
		}

	private:
#if defined(__EMSCRIPTEN__)
		void OpenGamepadIfNeeded()
		{
			if (m_pGamepad)
				return;

			int count = 0;
			SDL_JoystickID* gamepads = SDL_GetGamepads(&count);

			if (!gamepads)
			{
				return;
			}

			if (m_ControllerIndex < count)
			{
				m_pGamepad = SDL_OpenGamepad(gamepads[m_ControllerIndex]);
			}

			SDL_free(gamepads);
		}
#endif

		int m_ControllerIndex{};
		bool m_IsConnected{ false };

#if defined(__EMSCRIPTEN__)
		SDL_Gamepad* m_pGamepad{ nullptr };
		std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> m_PreviousButtons{};
		std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> m_CurrentButtons{};
#else
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		int m_ButtonsPressedThisFrame{};
		int m_ButtonsReleasedThisFrame{};
#endif
	};

	Controller::Controller(int controllerIndex)
		: m_pImpl(std::make_unique<Impl>(controllerIndex))
	{
	}

	Controller::~Controller() = default;

	void Controller::Update()
	{
		m_pImpl->Update();
	}

	bool Controller::IsPressed(ControllerButton button) const
	{
		return m_pImpl->IsPressed(button);
	}

	bool Controller::IsDownThisFrame(ControllerButton button) const
	{
		return m_pImpl->IsDownThisFrame(button);
	}

	bool Controller::IsUpThisFrame(ControllerButton button) const
	{
		return m_pImpl->IsUpThisFrame(button);
	}
}