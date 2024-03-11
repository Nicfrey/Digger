#pragma once
#include <SDL_scancode.h>
#include <memory>
#include <vector>

#include "Singleton.h"

class Command;

enum TriggerType
{
	KeyDown,
	KeyUp,
	KeyPressed,
	Thumbs
};


struct InputActionKeyboard
{
	InputActionKeyboard(const std::shared_ptr<Command>& pCommand, SDL_Scancode button, const TriggerType& triggerType = KeyDown) : triggerType{ triggerType }, button{ button }, pCommand{ pCommand }
	{}
	TriggerType triggerType;
	SDL_Scancode button;
	std::shared_ptr<Command> pCommand;
};

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void BindCommand(const std::shared_ptr<Command>& pCommand, SDL_Scancode button, const TriggerType& triggerType = KeyDown);
	private:
		std::vector<InputActionKeyboard> m_InputsActionKeyboards;
	};

}
