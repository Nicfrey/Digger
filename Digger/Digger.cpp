#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include <windows.h>
#include <iostream>
#include <steam_api.h>
#include <Xinput.h>

#include "Controller.h"
#include "DiggerCommands.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Minigin.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SpriteComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Digger");

	auto go{ std::make_shared<dae::GameObject>() };
	const auto spritePlayer1{ std::make_shared<SpriteComponent>("SpritesPlayers.png",4,4) };
	go->AddComponent(spritePlayer1);
	std::shared_ptr moveUpCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{0,-1}) };
	std::shared_ptr moveDownCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{0,1}) };
	std::shared_ptr moveLeftCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{-1,0}) };
	std::shared_ptr moveRightCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{1,0}) };
	GamepadController* gamepadController{ new GamepadController{} };
	gamepadController->BindAction(moveUpCommand, XINPUT_GAMEPAD_DPAD_UP);
	gamepadController->BindAction(moveDownCommand, XINPUT_GAMEPAD_DPAD_DOWN);
	gamepadController->BindAction(moveLeftCommand, XINPUT_GAMEPAD_DPAD_LEFT);
	gamepadController->BindAction(moveRightCommand, XINPUT_GAMEPAD_DPAD_RIGHT);
	dae::InputManager::GetInstance().AddController(gamepadController);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	const auto spritePlayer2{ std::make_shared<SpriteComponent>("SpritesPlayers.png",4,4) };
	go->AddComponent(spritePlayer2);
	moveUpCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{0,-1});
	moveDownCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{0,1});
	moveLeftCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{-1,0});
	moveRightCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{1,0});
	dae::InputManager::GetInstance().BindCommand(moveUpCommand, SDL_SCANCODE_W);
	dae::InputManager::GetInstance().BindCommand(moveDownCommand, SDL_SCANCODE_S);
	dae::InputManager::GetInstance().BindCommand(moveLeftCommand, SDL_SCANCODE_A);
	dae::InputManager::GetInstance().BindCommand(moveRightCommand, SDL_SCANCODE_D);
	scene.Add(go);

}

int main()
{
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	std::cout << "Successfully initialized steam." << '\n';
    dae::Minigin engine{"../Data/" };
    engine.Run(load);
	SteamAPI_Shutdown();
	return 0;
}
