#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include "InputManager.h"

#include <iostream>

#include "Gamepad.h"


bool dae::InputManager::ProcessInput()
{
	Gamepad::GetInstance().ProcessInput();
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			for(const auto& inputAction: m_InputsActionKeyboards)
			{
				if(inputAction.triggerType == KeyPressed && pStates[inputAction.button])
				{
					inputAction.pCommand->Execute();
				}
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		if(e.type == SDL_KEYUP)
		{
			for (const auto& inputAction : m_InputsActionKeyboards)
			{
				if (inputAction.triggerType == KeyUp && pStates[inputAction.button])
				{
					inputAction.pCommand->Execute();
				}
			}
		}
		// etc...
		ImGui_ImplSDL2_ProcessEvent(&e);
	}
	
	for (const auto& inputAction : m_InputsActionKeyboards)
	{
		if (inputAction.triggerType == KeyDown && pStates[inputAction.button])
		{
			inputAction.pCommand->Execute();
		}
	}


	return true;
}

void dae::InputManager::BindCommand(const std::shared_ptr<Command>& pCommand, SDL_Scancode button,
	const TriggerType& triggerType)
{
	if (pCommand != nullptr)
	{
		m_InputsActionKeyboards.emplace_back(pCommand, button, triggerType);
	}
}

