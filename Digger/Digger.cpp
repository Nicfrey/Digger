#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include <windows.h>
#include <iostream>
#include <steam_api.h>
#include <Xinput.h>

#include "Achievement.h"
#include "BoxCollider2D.h"
#include "Controller.h"
#include "DiggerCommands.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "LevelComponent.h"
#include "Minigin.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "SpriteComponent.h"
#include "UIPlayerComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Digger");

	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto go{ std::make_shared<dae::GameObject>() };
	const auto spritePlayer1{ std::make_shared<SpriteComponent>("SpritesPlayers.png",4,4) };
	auto healthComponent{ std::make_shared<HealthComponent>() };
	auto scoreComponent{ std::make_shared<ScoreComponent>() };
	auto uiComponent{ std::make_shared<UIPlayerComponent>(fontSmall) };
	auto boxCollider{ std::make_shared<BoxCollider2D>(spritePlayer1->GetShape().width,spritePlayer1->GetShape().height) };
	uiComponent->SetPosition(0, 150);
	go->AddComponent(healthComponent);
	go->AddComponent(uiComponent);
	go->AddComponent(scoreComponent);
	go->AddComponent(spritePlayer1);
	go->AddComponent(boxCollider);
	std::shared_ptr moveUpCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{0,-1}) };
	std::shared_ptr moveDownCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{0,1}) };
	std::shared_ptr moveLeftCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{-1,0}) };
	std::shared_ptr moveRightCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{1,0}) };
	std::shared_ptr killPlayerCommand{ std::make_shared<KillPlayerCommand>(go.get()) };
	std::shared_ptr addScoreCommand{ std::make_shared<AddScorePlayerCommand>(go.get()) };


	GamepadController* gamepadController{ new GamepadController{} };
	gamepadController->BindAction(moveUpCommand, XINPUT_GAMEPAD_DPAD_UP);
	gamepadController->BindAction(moveDownCommand, XINPUT_GAMEPAD_DPAD_DOWN);
	gamepadController->BindAction(moveLeftCommand, XINPUT_GAMEPAD_DPAD_LEFT);
	gamepadController->BindAction(moveRightCommand, XINPUT_GAMEPAD_DPAD_RIGHT);
	gamepadController->BindAction(killPlayerCommand, XINPUT_GAMEPAD_X, KeyPressed);
	gamepadController->BindAction(addScoreCommand, XINPUT_GAMEPAD_A, KeyPressed);
	dae::InputManager::GetInstance().AddController(gamepadController);
	go->SetLocalPosition(20, 100);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	const auto spritePlayer2{ std::make_shared<SpriteComponent>("SpritesPlayers.png",4,4) };
	healthComponent = std::make_shared<HealthComponent>();
	uiComponent = std::make_shared<UIPlayerComponent>(fontSmall);
	scoreComponent = std::make_shared<ScoreComponent>();
	boxCollider = std::make_shared<BoxCollider2D>(spritePlayer2->GetShape().width,spritePlayer2->GetShape().height);
	uiComponent->SetPosition(0, 210);
	go->AddComponent(spritePlayer2);
	go->AddComponent(healthComponent);
	go->AddComponent(uiComponent);
	go->AddComponent(scoreComponent);
	go->AddComponent(boxCollider);
	go->SetTag("Player");
	moveUpCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{0,-1});
	moveDownCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{0,1});
	moveLeftCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{-1,0});
	moveRightCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{1,0});
	killPlayerCommand = std::make_shared<KillPlayerCommand>(go.get());
	addScoreCommand = std::make_shared<AddScorePlayerCommand>(go.get());
	dae::InputManager::GetInstance().BindCommand(moveUpCommand, SDL_SCANCODE_W);
	dae::InputManager::GetInstance().BindCommand(moveDownCommand, SDL_SCANCODE_S);
	dae::InputManager::GetInstance().BindCommand(moveLeftCommand, SDL_SCANCODE_A);
	dae::InputManager::GetInstance().BindCommand(moveRightCommand, SDL_SCANCODE_D);
	dae::InputManager::GetInstance().BindCommand(killPlayerCommand, SDL_SCANCODE_X, KeyPressed);
	dae::InputManager::GetInstance().BindCommand(addScoreCommand, SDL_SCANCODE_Z, KeyPressed);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_shared<LevelComponent>());
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
	Achievement_t g_Achievements[] =
	{
		_ACH_ID(ACH_WIN_ONE_GAME, "Vainqueur"),
		_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
		_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellaire"),
		_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Étoile"),
	};

	const auto achievement = new Achievement(g_Achievements, 4);
    dae::Minigin engine{"../Data/" };
    engine.Run(load);
	SteamAPI_Shutdown();
	delete achievement;
	return 0;
}
