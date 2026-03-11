#include "Controller.h"

#include <Windows.h>
#include <Xinput.h>

namespace
{
	WORD ToXInputButton(dae::ControllerButton button)
	{
		switch (button)
		{
		case dae::ControllerButton::DPadUp:    
			return XINPUT_GAMEPAD_DPAD_UP;
		case dae::ControllerButton::DPadDown:  
			return XINPUT_GAMEPAD_DPAD_DOWN;
		case dae::ControllerButton::DPadLeft:  
			return XINPUT_GAMEPAD_DPAD_LEFT;
		case dae::ControllerButton::DPadRight: 
			return XINPUT_GAMEPAD_DPAD_RIGHT;
		case dae::ControllerButton::A:         
			return XINPUT_GAMEPAD_A;
		case dae::ControllerButton::B:         
			return XINPUT_GAMEPAD_B;
		case dae::ControllerButton::X:         
			return XINPUT_GAMEPAD_X;
		case dae::ControllerButton::Y:         
			return XINPUT_GAMEPAD_Y;
		default: return 0;
		}
	}
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

		void Update()
		{
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

			const auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
		}

		bool IsPressed(ControllerButton button) const
		{
			const WORD xButton = ToXInputButton(button);
			return (m_CurrentState.Gamepad.wButtons & xButton) != 0;
		}

		bool IsDownThisFrame(ControllerButton button) const
		{
			const WORD xButton = ToXInputButton(button);
			return (m_ButtonsPressedThisFrame & xButton) != 0;
		}

		bool IsUpThisFrame(ControllerButton button) const
		{
			const WORD xButton = ToXInputButton(button);
			return (m_ButtonsReleasedThisFrame & xButton) != 0;
		}

	private:
		int m_ControllerIndex{};
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};

		int m_ButtonsPressedThisFrame{};
		int m_ButtonsReleasedThisFrame{};
		bool m_IsConnected{ false };
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