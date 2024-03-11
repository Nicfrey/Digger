#pragma once
#include <list>
#include <SDL_syswm.h>
#include <Xinput.h>

#include "Command.h"
#include "Singleton.h"

enum TriggerTypeXInput
{
	Down,
	Up,
	Pressed,
	Thumbs
};

struct InputAction
{
	InputAction(Command* pCommand, unsigned int button, TriggerTypeXInput triggerType = Pressed) : triggerType{triggerType}, button{button}, pCommand{pCommand}
	{}
	TriggerTypeXInput triggerType;
	unsigned int button;
	Command* pCommand;
};

class Gamepad final : public dae::Singleton<Gamepad>
{
public:
	void ProcessInput();
	void BindCommand(Command* pCommand, unsigned int button, const TriggerTypeXInput& triggerType = Pressed);
private:
	DWORD m_ControllerIndex{};
	int m_ButtonsPressedThisFrame{};
	int m_ButtonsReleasedThisFrame{};

	XINPUT_STATE m_PreviousState;
	XINPUT_STATE m_CurrentState;

	float m_PercentageDeadZone{ 20.f };
	std::list<InputAction> m_InputActions;

	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;
	bool IsThumbsNotInDeadZone() const;
};

