#pragma once
#include <list>
#include <memory>
#include <SDL_syswm.h>
#include <Xinput.h>

#include "Command.h"
#include "InputManager.h"
#include "Singleton.h"

struct InputActionController
{
	InputActionController(const std::shared_ptr<Command>& pCommand, unsigned int button, const TriggerType& triggerType = KeyDown) : triggerType{triggerType}, button{button}, pCommand{pCommand}
	{}
	TriggerType triggerType;
	unsigned int button;
	std::shared_ptr<Command> pCommand;
};

class Gamepad final : public dae::Singleton<Gamepad>
{
public:
	void ProcessInput();
	void BindCommand(const std::shared_ptr<Command>& pCommand, unsigned int button, const TriggerType& triggerType = KeyDown);
private:
	DWORD m_ControllerIndex{};
	int m_ButtonsPressedThisFrame{};
	int m_ButtonsReleasedThisFrame{};

	XINPUT_STATE m_PreviousState;
	XINPUT_STATE m_CurrentState;

	float m_PercentageDeadZone{ 20.f };
	std::list<InputActionController> m_InputActions;

	bool IsPressedThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsDown(unsigned int button) const;
	bool IsThumbsNotInDeadZone() const;
};

