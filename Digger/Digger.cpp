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
#include "AnimatorComponent.h"
#include "BoxCollider2D.h"
#include "Controller.h"
#include "DiggerCommands.h"
#include "DiggerTransitionAnim.h"
#include "DiggerUtils.h"
#include "EmeraldComponent.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "LevelComponent.h"
#include "Minigin.h"
#include "PlayerComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "SoundSystemEngine.h"
#include "SpriteComponent.h"
#include "UIPlayerComponent.h"

void load()
{
	ServiceSoundLocator::RegisterSoundSystem(std::make_unique<SoundSystemEngine>());
	auto& ss{ ServiceSoundLocator::GetSoundSystem() };
	ss.Add(static_cast<SoundId>(DiggerUtils::SoundDiggerID::PROJECTILE_HIT),"Sounds/ProjectileHit.wav");

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Digger");
	dae::SceneManager::GetInstance().SetActiveScene("Digger");

	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto go{ std::make_shared<dae::GameObject>() };
	const auto spritePlayer1{ std::make_shared<SpriteComponent>("SpritesPlayers.png",4,4) };
	auto healthComponent{ std::make_shared<HealthComponent>() };
	auto scoreComponent{ std::make_shared<ScoreComponent>() };
	auto uiComponent{ std::make_shared<UIPlayerComponent>(fontSmall) };
	auto boxCollider{ std::make_shared<BoxCollider2D>(spritePlayer1->GetShape().width,spritePlayer1->GetShape().height) };
	auto playerComponent{ std::make_shared<PlayerComponent>() };
	auto animator{ std::make_shared<AnimatorComponent>() };
	Animation idlePlayer{ .name = "Idle",.frames = {0,1,2},.frameTime = 0.1f, .spriteComponent = spritePlayer1 };
	Animation idleWithoutShoot{ .name = "IdleWithoutShoot",.frames{4,5,6},.frameTime = 0.1f,.spriteComponent = spritePlayer1 };
	Animation deadAnim{ .name = "DeadAnim", .frames = {3},.spriteComponent = spritePlayer1 };
	TransitionNoProjectile* transitionNoProjectile{ new TransitionNoProjectile() };
	TransitionProjectile* transitionProjectile{ new TransitionProjectile{} };
	TransitionDead* transitionDead{ new TransitionDead{} };
	animator->AddTransition(idlePlayer, idleWithoutShoot, transitionNoProjectile);
	animator->AddTransition(idleWithoutShoot, idlePlayer, transitionProjectile);
	animator->AddTransition(idlePlayer, deadAnim, transitionDead);
	animator->AddTransition(idleWithoutShoot, deadAnim, transitionDead);
	if (!animator->SetStartAnimation(idlePlayer))
	{
		std::cerr << "Failed to set start animation" << '\n';
	}
	uiComponent->SetPosition(0, 150);
	go->AddComponent(healthComponent);
	go->AddComponent(uiComponent);
	go->AddComponent(scoreComponent);
	go->AddComponent(spritePlayer1);
	go->AddComponent(boxCollider);
	go->AddComponent(playerComponent);
	go->AddComponent(animator);
	std::shared_ptr moveUpCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{0,-1}) };
	std::shared_ptr moveDownCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{0,1}) };
	std::shared_ptr moveLeftCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{-1,0}) };
	std::shared_ptr moveRightCommand{ std::make_shared<MoveCommand>(go.get(),glm::vec2{1,0}) };
	std::shared_ptr killPlayerCommand{ std::make_shared<KillPlayerCommand>(go.get()) };
	std::shared_ptr addScoreCommand{ std::make_shared<AddScorePlayerCommand>(go.get()) };
	std::shared_ptr shootCommand{ std::make_shared<ShootCommand>(go.get()) };

	GamepadController* gamepadController{ new GamepadController{} };
	gamepadController->BindAction(moveUpCommand, XINPUT_GAMEPAD_DPAD_UP);
	gamepadController->BindAction(moveDownCommand, XINPUT_GAMEPAD_DPAD_DOWN);
	gamepadController->BindAction(moveLeftCommand, XINPUT_GAMEPAD_DPAD_LEFT);
	gamepadController->BindAction(moveRightCommand, XINPUT_GAMEPAD_DPAD_RIGHT);
	gamepadController->BindAction(killPlayerCommand, XINPUT_GAMEPAD_X, KeyPressed);
	gamepadController->BindAction(addScoreCommand, XINPUT_GAMEPAD_A, KeyPressed);
	gamepadController->BindAction(shootCommand, XINPUT_GAMEPAD_B, KeyPressed);
	dae::InputManager::GetInstance().AddController(gamepadController);
	go->SetLocalPosition(20, 100);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	const auto spritePlayer2{ std::make_shared<SpriteComponent>("SpritesPlayers.png",4,4) };
	healthComponent = std::make_shared<HealthComponent>();
	uiComponent = std::make_shared<UIPlayerComponent>(fontSmall);
	scoreComponent = std::make_shared<ScoreComponent>();
	boxCollider = std::make_shared<BoxCollider2D>(spritePlayer2->GetShape().width,spritePlayer2->GetShape().height);
	playerComponent = std::make_shared<PlayerComponent>();
	animator = std::make_shared<AnimatorComponent>();
	idlePlayer.frames = { 8,9,10 };
	idlePlayer.spriteComponent = spritePlayer2;
	idleWithoutShoot.spriteComponent = spritePlayer2;
	idleWithoutShoot.frames = { 12,13,14 };
	deadAnim.frames = { 11 };
	deadAnim.spriteComponent = spritePlayer2;
	transitionNoProjectile = new TransitionNoProjectile{};
	transitionProjectile = new TransitionProjectile{};
	transitionDead = new TransitionDead{};
	animator->AddTransition(idlePlayer,idleWithoutShoot,transitionNoProjectile);
	animator->AddTransition(idleWithoutShoot,idlePlayer,transitionProjectile);
	animator->AddTransition(idlePlayer,deadAnim,transitionDead);
	animator->AddTransition(idleWithoutShoot,deadAnim,transitionDead);
	if(!animator->SetStartAnimation(idlePlayer))
	{
		std::cerr << "Failed to set start animation" << '\n';
	}
	uiComponent->SetPosition(0, 210);
	go->AddComponent(spritePlayer2);
	go->AddComponent(healthComponent);
	go->AddComponent(uiComponent);
	go->AddComponent(scoreComponent);
	go->AddComponent(boxCollider);
	go->AddComponent(playerComponent);
	go->AddComponent(animator);
	go->SetTag("Player");
	moveUpCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{0,-1});
	moveDownCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{0,1});
	moveLeftCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{-1,0});
	moveRightCommand = std::make_shared<MoveCommand>(go.get(),glm::vec2{1,0});
	killPlayerCommand = std::make_shared<KillPlayerCommand>(go.get());
	addScoreCommand = std::make_shared<AddScorePlayerCommand>(go.get());
	shootCommand = std::make_shared<ShootCommand>(go.get());
	dae::InputManager::GetInstance().BindCommand(moveUpCommand, SDL_SCANCODE_W);
	dae::InputManager::GetInstance().BindCommand(moveDownCommand, SDL_SCANCODE_S);
	dae::InputManager::GetInstance().BindCommand(moveLeftCommand, SDL_SCANCODE_A);
	dae::InputManager::GetInstance().BindCommand(moveRightCommand, SDL_SCANCODE_D);
	dae::InputManager::GetInstance().BindCommand(killPlayerCommand, SDL_SCANCODE_X, KeyPressed);
	dae::InputManager::GetInstance().BindCommand(addScoreCommand, SDL_SCANCODE_Z, KeyPressed);
	dae::InputManager::GetInstance().BindCommand(shootCommand, SDL_SCANCODE_SPACE, KeyPressed);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto sprite{ std::make_shared<SpriteComponent>("SpritesEnemies.png",4,2) };
	healthComponent = std::make_shared<HealthComponent>(1);
	boxCollider = std::make_shared<BoxCollider2D>(sprite->GetShape().width,sprite->GetShape().height);
	auto enemyComponent{ std::make_shared<EnemyComponent>(EnemyComponent::EnemyType::Nobbins) };
	go->AddComponent(sprite);
	go->AddComponent(healthComponent);
	go->AddComponent(boxCollider);
	go->AddComponent(enemyComponent);
	go->SetLocalPosition(50, 150);
	go->SetTag("Enemy");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	sprite = std::make_shared<SpriteComponent>("SpritesEnemies.png",4,2);
	healthComponent = std::make_shared<HealthComponent>(1);
	boxCollider = std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height);
	enemyComponent = std::make_shared<EnemyComponent>(EnemyComponent::EnemyType::Hobbins);
	go->AddComponent(sprite);
	go->AddComponent(healthComponent);
	go->AddComponent(boxCollider);
	go->AddComponent(enemyComponent);
	go->SetLocalPosition(50, 180);
	go->SetTag("Enemy");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	sprite = std::make_shared<SpriteComponent>("SpritesItems.png",3,3);
	boxCollider = std::make_shared<BoxCollider2D>(sprite->GetShape().width,sprite->GetShape().height);
	const auto item{ std::make_shared<EmeraldComponent>() };
	go->AddComponent(sprite);
	go->AddComponent(boxCollider);
	go->AddComponent(item);
	go->SetLocalPosition(50, 210);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_shared<LevelComponent>());
	scene.Add(go);

	auto fontTiny = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);
	go = std::make_shared<dae::GameObject>();
	auto text = std::make_shared<dae::TextComponent>("Press WASD to move and SPACEBAR to shoot projectile to enemy or player", fontTiny);
	go->SetLocalPosition(0, 20);
	go->AddComponent(text);
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
