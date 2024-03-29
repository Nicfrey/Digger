#include <steam_api.h>
#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include <windows.h>
#include <Xinput.h>
#include "FpsTextComponent.h"
#include "GameObjectCommand.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "Minigin.h"
#include "MovementComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "ScoreComponent.h"
#include "TextureComponent.h"
#include "TrashTheCacheGOComponent.h"
#include "UIPlayerComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	const auto tmBackground{std::make_shared<TextureComponent>()};
	tmBackground->SetTexture("background.tga");
	go->AddComponent(tmBackground);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>(glm::vec3{216,180,0.f});
	const std::shared_ptr<TextureComponent> tmLogo{ std::make_shared<TextureComponent>()};
	tmLogo->SetTexture("logo.tga");
	go->AddComponent(tmLogo);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>();
	const auto textManager{ std::make_shared<dae::TextComponent>("Programming 4 Assignment",font) };
	go->SetLocalPosition(80, 20);
	go->AddComponent(textManager);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	const auto textManagerFPS{ std::make_shared<FpsTextComponent>(font) };
	go->SetLocalPosition(0, 20);
	go->AddComponent(textManagerFPS);
	scene.Add(go);

	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	const auto goParent = std::make_shared<dae::GameObject>();
	const auto characterText{ std::make_shared<TextureComponent>() };
	characterText->SetTexture("MainCharacter.png");
	auto movement{ std::make_shared<MovementComponent>() };
	auto healthComponent{ std::make_shared<HealthComponent>() };
	auto scoreComponent{ std::make_shared<ScoreComponent>() };
	auto uiComponent{ std::make_shared<UIPlayerComponent>(fontSmall) };
	uiComponent->SetPosition(0, 150);
	goParent->SetLocalPosition(217,200);
	goParent->AddComponent(characterText);
	goParent->AddComponent(movement);
	goParent->AddComponent(healthComponent);
	goParent->AddComponent(uiComponent);
	goParent->AddComponent(scoreComponent);


	std::shared_ptr moveForwardCommand{ std::make_shared<MoveForwardCommand>(goParent.get()) };
	std::shared_ptr moveBackwardCommand{ std::make_shared<MoveBackwardCommand>(goParent.get()) };
	std::shared_ptr moveLeftCommand{ std::make_shared<MoveLeftCommand>(goParent.get()) };
	std::shared_ptr moveRightCommand{ std::make_shared<MoveRightCommand>(goParent.get()) };
	std::shared_ptr killPlayerCommand{ std::make_shared<KillPlayerCommand>(goParent.get()) };
	std::shared_ptr addScoreCommand{ std::make_shared<AddScorePlayerCommand>(goParent.get()) };

	GamepadController* gamepadController{ new GamepadController{} };
	gamepadController->BindAction(moveForwardCommand, XINPUT_GAMEPAD_DPAD_UP);
	gamepadController->BindAction(moveBackwardCommand, XINPUT_GAMEPAD_DPAD_DOWN);
	gamepadController->BindAction(moveLeftCommand, XINPUT_GAMEPAD_DPAD_LEFT);
	gamepadController->BindAction(moveRightCommand, XINPUT_GAMEPAD_DPAD_RIGHT);
	gamepadController->BindAction(killPlayerCommand, XINPUT_GAMEPAD_X,KeyPressed);
	gamepadController->BindAction(addScoreCommand, XINPUT_GAMEPAD_A, KeyPressed);

	dae::InputManager::GetInstance().AddController(gamepadController);
	scene.Add(goParent);

	go = std::make_shared<dae::GameObject>();
	const auto secondCharacterText{ std::make_shared<TextureComponent>() };
	secondCharacterText->SetTexture("SecondCharacter.png");
	movement = std::make_shared<MovementComponent>();
	healthComponent = std::make_shared<HealthComponent>();
	uiComponent = std::make_shared<UIPlayerComponent>(fontSmall);
	scoreComponent = std::make_shared<ScoreComponent>();
	uiComponent->SetPosition(0, 210);
	go->SetLocalPosition(0, 20);
	go->AddComponent(secondCharacterText);
	go->AddComponent(movement);
	go->AddComponent(healthComponent);
	go->AddComponent(uiComponent);
	go->AddComponent(scoreComponent);

	moveForwardCommand = std::make_shared<MoveForwardCommand>(go.get());
	moveBackwardCommand = std::make_shared<MoveBackwardCommand>(go.get());
	moveLeftCommand = std::make_shared<MoveLeftCommand>(go.get());
	moveRightCommand = std::make_shared<MoveRightCommand>(go.get());
	killPlayerCommand = std::make_shared<KillPlayerCommand>(go.get());
	addScoreCommand = std::make_shared<AddScorePlayerCommand>(go.get());
	dae::InputManager::GetInstance().BindCommand(moveForwardCommand, SDL_SCANCODE_W);
	dae::InputManager::GetInstance().BindCommand(moveBackwardCommand, SDL_SCANCODE_S);
	dae::InputManager::GetInstance().BindCommand(moveLeftCommand, SDL_SCANCODE_A);
	dae::InputManager::GetInstance().BindCommand(moveRightCommand, SDL_SCANCODE_D);
	dae::InputManager::GetInstance().BindCommand(killPlayerCommand, SDL_SCANCODE_X, KeyPressed);
	dae::InputManager::GetInstance().BindCommand(addScoreCommand, SDL_SCANCODE_Z, KeyPressed);

	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto textController{ std::make_shared<dae::TextComponent>("Use the D-PAD to move the big eye digger, X to inflict damage, A to add score",fontSmall) };
	go->SetLocalPosition(0, 80);
	go->AddComponent(textController);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	textController = std::make_shared<dae::TextComponent>("Use WASD to move the small eye digger, X to inflict damage, W to add score",fontSmall);
	go->SetLocalPosition(0, 100);
	go->AddComponent(textController);
	scene.Add(go);
}

int main(int, char*[]) {
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	std::cout << "Successfully initialized steam." << std::endl;
	dae::Minigin engine("../Data/");
	engine.Run(load);
	SteamAPI_Shutdown();
    return 0;
}