#include "Controller.h"

dae::Controller::Controller(int controllerIndex):
	m_ControllerIndex(controllerIndex)
{

}

{
//Bit operators reminder!!
// ^  = XOR (bitwise exclusive OR) -> detects bits that changed
// &  = AND (keeps only bits that exist in both values)
// ~  = NOT (flips all bits)
}

void dae::Controller::Update()
{
	m_PreviousState = m_CurrentState;
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

	//My extra check - if we don't have controller pluged in, just set it to 0
	DWORD result = XInputGetState(m_ControllerIndex, &m_CurrentState);
	if (result == ERROR_SUCCESS)
	{
		m_IsConnected = true;
	}
	else {
		m_IsConnected = false;
	}

	if (!m_IsConnected)
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		m_ButtonsPressedThisFrame = 0;
		m_ButtonsReleasedThisFrame = 0;
		return;
	}

	// XOR (^) compares the current and previous button states.
	// It returns bits that changed between frames (either 0->1 or 1->0)
	const auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	// From the changed buttons, keep only those that are currently pressed
	// These are the buttons that went from 0 -> 1 this frame
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	// ~ is bitwise NOT (it flips all bits)
	// Here we take the changed buttons and keep only those that are NOT pressed now
	// These are the buttons that went from 1 -> 0 this frame
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool dae::Controller::IsPressed(int button) const
{
	return (m_CurrentState.Gamepad.wButtons & button) != 0;
}

bool dae::Controller::IsDownThisFrame(int button) const
{
	return (m_ButtonsPressedThisFrame & button) != 0;
}

bool dae::Controller::IsUpThisFrame(int button) const
{
	return (m_ButtonsReleasedThisFrame & button) != 0;
}
