#include "Gamepad.h"
#include <SDL_syswm.h>
#include <Xinput.h>

void Gamepad::ProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	int buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

	for(const InputAction& inputAction: m_InputActions)
	{
		switch (inputAction.triggerType)
		{
		case Down:
			if(IsDownThisFrame(inputAction.button))
			{
				inputAction.pCommand->Execute();
			}
			break;
		case Up:
			if(IsUpThisFrame(inputAction.button))
			{
				inputAction.pCommand->Execute();
			}
			break;
		case Pressed:
			if(IsPressed(inputAction.button))
			{
				inputAction.pCommand->Execute();
			}
			break;
		case Thumbs:
			if(IsThumbsNotInDeadZone())
			{
				inputAction.pCommand->Execute(); // needs modification later on
			}
			break;
		}
	}
}

void Gamepad::BindCommand(Command* pCommand,unsigned int button, const TriggerTypeXInput& triggerType)
{
	if(pCommand != nullptr)
	{
		m_InputActions.emplace_back(pCommand,button,triggerType);
	}
}

bool Gamepad::IsDownThisFrame(unsigned button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool Gamepad::IsUpThisFrame(unsigned button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool Gamepad::IsPressed(unsigned button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

bool Gamepad::IsThumbsNotInDeadZone() const
{
	const SHORT thumbL{ m_CurrentState.Gamepad.sThumbLX };
	const SHORT thumbR{ m_CurrentState.Gamepad.sThumbRX };
	const float percentageThumbL{ std::abs(thumbL / static_cast<float>(SHRT_MIN)) };
	const float percentageThumbR{ std::abs(thumbR / static_cast<float>(SHRT_MAX)) };

	return percentageThumbL > m_PercentageDeadZone || percentageThumbR > m_PercentageDeadZone;
}
