#include "XInputController.h"
#include "InputManager.h"

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

//-----CONTROLLER IMPLEMENTATION-----
class dae::XInputController::XInputControllerImpl
{
public:
	XInputControllerImpl()
		: m_ControllerIdx{ m_NrOfControllers }
	{
		++m_NrOfControllers;

		ZeroMemory(&m_PreviousState, sizeof(m_PreviousState));
		ZeroMemory(&m_CurrentState, sizeof(m_CurrentState));
	}

	void Update();

	bool WentDownThisFrame(ControllerButtons button) const;
	bool WentUpThisFrame(ControllerButtons button) const;
	bool IsPressed(ControllerButtons button) const;

	int GetIndex() const { return static_cast<int>(m_ControllerIdx); }

private:
	static DWORD m_NrOfControllers;
	DWORD m_ControllerIdx;

	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};
};

DWORD dae::XInputController::XInputControllerImpl::m_NrOfControllers = 0;

void dae::XInputController::XInputControllerImpl::Update()
{
	if (m_ControllerIdx >= 0)
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(m_CurrentState));
		ZeroMemory(&m_CurrentState, sizeof(m_CurrentState));
		XInputGetState(m_ControllerIdx, &m_CurrentState);

		auto buttonChanges = m_PreviousState.Gamepad.wButtons ^ m_CurrentState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}
}

bool dae::XInputController::XInputControllerImpl::WentDownThisFrame(ControllerButtons button) const
{
	if (m_ButtonsPressedThisFrame == static_cast<unsigned int>(button))
		return true;

	return false;
}

bool dae::XInputController::XInputControllerImpl::WentUpThisFrame(ControllerButtons button) const
{
	if (m_ButtonsReleasedThisFrame == static_cast<unsigned int>(button))
		return true;

	return false;
}

bool dae::XInputController::XInputControllerImpl::IsPressed(ControllerButtons button) const
{
	unsigned int buttonU = static_cast<unsigned int>(button);

	if (m_ButtonsReleasedThisFrame != buttonU && m_CurrentState.Gamepad.wButtons == buttonU)
		return true;

	return false;
}

//-----CONTROLLER-----
dae::XInputController::XInputController()
	: m_pImpl{ new XInputControllerImpl() }
{
}

dae::XInputController::~XInputController()
{
	delete m_pImpl;
}

void dae::XInputController::Update()
{
	m_pImpl->Update();
}

bool dae::XInputController::WentDownThisFrame(ControllerButtons button) const
{
	return m_pImpl->WentDownThisFrame(button);
}

bool dae::XInputController::WentUpThisFrame(ControllerButtons button) const
{
	return m_pImpl->WentUpThisFrame(button);
}

bool dae::XInputController::IsPressed(ControllerButtons button) const
{
	return m_pImpl->IsPressed(button);
}

int dae::XInputController::GetIndex() const
{
	return m_pImpl->GetIndex();
}