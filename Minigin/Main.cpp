#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "FpsTextComponent.h"
#include "GameObjectCommand.h"
#include "Gamepad.h"
#include "Minigin.h"
#include "MovementComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "TrashTheCacheGOComponent.h"

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

	const auto goParent = std::make_shared<dae::GameObject>();
	const auto characterText{ std::make_shared<TextureComponent>() };
	characterText->SetTexture("MainCharacter.png");
	auto movement{ std::make_shared<MovementComponent>() };
	goParent->SetLocalPosition(217,200);
	goParent->AddComponent(characterText);
	goParent->AddComponent(movement);

	std::shared_ptr moveForwardCommand{ std::make_shared<MoveForwardCommand>(goParent.get()) };
	std::shared_ptr moveBackwardCommand{ std::make_shared<MoveBackwardCommand>(goParent.get()) };
	std::shared_ptr moveLeftCommand{ std::make_shared<MoveLeftCommand>(goParent.get()) };
	std::shared_ptr moveRightCommand{ std::make_shared<MoveRightCommand>(goParent.get()) };
	Gamepad::GetInstance().BindCommand(moveForwardCommand, XINPUT_GAMEPAD_DPAD_UP);
	Gamepad::GetInstance().BindCommand(moveBackwardCommand, XINPUT_GAMEPAD_DPAD_DOWN);
	Gamepad::GetInstance().BindCommand(moveLeftCommand, XINPUT_GAMEPAD_DPAD_LEFT);
	Gamepad::GetInstance().BindCommand(moveRightCommand, XINPUT_GAMEPAD_DPAD_RIGHT);
	scene.Add(goParent);

	go = std::make_shared<dae::GameObject>();
	const auto secondCharacterText{ std::make_shared<TextureComponent>() };
	secondCharacterText->SetTexture("SecondCharacter.png");
	movement = std::make_shared<MovementComponent>();
	go->SetLocalPosition(0, 20);
	go->AddComponent(secondCharacterText);
	go->AddComponent(movement);

	moveForwardCommand = std::make_shared<MoveForwardCommand>(go.get());
	moveBackwardCommand = std::make_shared<MoveBackwardCommand>(go.get());
	moveLeftCommand = std::make_shared<MoveLeftCommand>(go.get());
	moveRightCommand = std::make_shared<MoveRightCommand>(go.get());
	dae::InputManager::GetInstance().BindCommand(moveForwardCommand, SDL_SCANCODE_W);
	dae::InputManager::GetInstance().BindCommand(moveBackwardCommand, SDL_SCANCODE_S);
	dae::InputManager::GetInstance().BindCommand(moveLeftCommand, SDL_SCANCODE_A);
	dae::InputManager::GetInstance().BindCommand(moveRightCommand, SDL_SCANCODE_D);

	scene.Add(go);

	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	go = std::make_shared<dae::GameObject>();
	auto textController{ std::make_shared<dae::TextComponent>("Use the D-PAD to move the big eye digger",fontSmall) };
	go->SetLocalPosition(0, 80);
	go->AddComponent(textController);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	textController = std::make_shared<dae::TextComponent>("Use WASD to move the small eye digger",fontSmall);
	go->SetLocalPosition(0, 100);
	go->AddComponent(textController);
	scene.Add(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}