#pragma once
#include <SDL_scancode.h>
#include <memory>
#include <vector>

#include "Controller.h"
#include "Singleton.h"

class Command;


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
		void UnbindCommandObjects();
		void UnbindCommandObjectsDestroyed();
		template<typename T>
		void AddController(T* controller);
	private:
		std::vector<InputActionKeyboard> m_InputsActionKeyboards;
		std::vector<std::unique_ptr<Controller>> m_Controllers;
	};

	template <typename T>
	void InputManager::AddController(T* controller)
	{
		if(std::is_base_of_v<Controller,T>)
		{
			m_Controllers.emplace_back(std::move(controller));
		}
	}
}
