#include "Controller.h"

#include <list>
#include <memory>
#include <SDL_syswm.h>
#include <Xinput.h>

#include "Command.h"
#include "GameObjectCommand.h"
#include "InputManager.h"

class GamepadController::GamepadControllerImpl
{
	public:
		GamepadControllerImpl(int index);
		~GamepadControllerImpl();

		struct InputActionController
		{
			InputActionController(const std::shared_ptr<Command>& pCommand, unsigned int button, const TriggerType& triggerType = KeyDown) : triggerType{ triggerType }, button{ button }, pCommand{ pCommand }
			{}
			TriggerType triggerType;
			unsigned int button;
			std::shared_ptr<Command> pCommand;
		};

		void DoProcessInput();
		void DoBindAction(const std::shared_ptr<Command>& pCommand, unsigned int button, const TriggerType& triggerType = KeyDown);
		void DoUnbindActionGameObject();

private:
	static DWORD m_TotalController;
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

DWORD GamepadController::GamepadControllerImpl::m_TotalController = 0;

GamepadController::GamepadControllerImpl::GamepadControllerImpl(int index)
{
	m_ControllerIndex = index;
}

GamepadController::GamepadControllerImpl::~GamepadControllerImpl()
{
	--m_TotalController;
}

void GamepadController::GamepadControllerImpl::DoUnbindActionGameObject()
{
	std::erase_if(m_InputActions, [](const InputActionController& inputAction) { return std::dynamic_pointer_cast<GameObjectCommand>(inputAction.pCommand); });
}

void GamepadController::GamepadControllerImpl::DoProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	const int buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

	for (const InputActionController& inputAction : m_InputActions)
	{
		switch (inputAction.triggerType)
		{
		case KeyDown:
			if (IsDown(inputAction.button))
			{
				inputAction.pCommand->Execute();
			}
			break;
		case KeyUp:
			if (IsUpThisFrame(inputAction.button))
			{
				inputAction.pCommand->Execute();
			}
			break;
		case KeyPressed:
			if (IsPressedThisFrame(inputAction.button))
			{
				inputAction.pCommand->Execute();
			}
			break;
		case Thumbs:
			if (IsThumbsNotInDeadZone())
			{
				inputAction.pCommand->Execute(); // needs modification later on
			}
			break;
		}
	}
}

void GamepadController::GamepadControllerImpl::DoBindAction(const std::shared_ptr<Command>& pCommand, unsigned button,
	const TriggerType& triggerType)
{
	if (pCommand != nullptr)
	{
		m_InputActions.emplace_back(pCommand, button, triggerType);
	}
}

bool GamepadController::GamepadControllerImpl::IsPressedThisFrame(unsigned button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool GamepadController::GamepadControllerImpl::IsUpThisFrame(unsigned button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool GamepadController::GamepadControllerImpl::IsDown(unsigned button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

bool GamepadController::GamepadControllerImpl::IsThumbsNotInDeadZone() const
{
	const SHORT thumbL{ m_CurrentState.Gamepad.sThumbLX };
	const SHORT thumbR{ m_CurrentState.Gamepad.sThumbRX };
	const float percentageThumbL{ std::abs(thumbL / static_cast<float>(SHRT_MIN)) };
	const float percentageThumbR{ std::abs(thumbR / static_cast<float>(SHRT_MAX)) };

	return percentageThumbL > m_PercentageDeadZone || percentageThumbR > m_PercentageDeadZone;
}

GamepadController::GamepadController(int index)
{
	m_pImpl = new GamepadControllerImpl{index};
}

GamepadController::~GamepadController()
{
	delete m_pImpl;
	m_pImpl = nullptr;
}

void GamepadController::ProcessInput()
{
	m_pImpl->DoProcessInput();
}

void GamepadController::BindAction(const std::shared_ptr<Command>& pCommand, unsigned button,
	const TriggerType& triggerType)
{
	m_pImpl->DoBindAction(pCommand, button, triggerType);
}

void GamepadController::UnbindActionGameObject()
{
	m_pImpl->DoUnbindActionGameObject();
}
