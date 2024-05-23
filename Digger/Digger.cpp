#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include <windows.h>
#include <iostream>

#include "ButtonComponent.h"
#include "Controller.h"
#include "DiggerUtils.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameStateComponent.h"
#include "InputManager.h"
#include "Minigin.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SoundSystemEngine.h"
#include "Widget.h"
#include "WidgetManager.h"

void load()
{
	dae::ResourceManager::GetInstance().Init("../Data/");
#if _DEBUG
	ServiceSoundLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SoundSystemEngine>()));
	ServiceMusicLocator::RegisterMusicSystem(std::make_unique<LoggingMusicSystem>(std::make_unique<MusicSystemEngine>()));
#else
	ServiceSoundLocator::RegisterSoundSystem(std::make_unique<SoundSystemEngine>());
	ServiceMusicLocator::RegisterMusicSystem(std::make_unique<MusicSystemEngine>());
#endif
	auto& ss{ ServiceSoundLocator::GetSoundSystem() };
	auto& ms{ ServiceMusicLocator::GetMusicSystem() };
	ss.Add(static_cast<SoundId>(DiggerUtils::SoundDiggerID::PROJECTILE_HIT), "Sounds/ProjectileHit.wav");
	ms.Add(static_cast<MusicId>(DiggerUtils::MusicDiggerID::GAME),"Sounds/DiggerMusic.mp3");
	auto& scene = dae::SceneManager::GetInstance().CreateScene("MenuDigger");
	dae::SceneManager::GetInstance().SetActiveScene("MenuDigger");

	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);


	/*
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
	transitionNoProjectile = new TransitionPlayerNoProjectile{};
	transitionProjectile = new TransitionPlayerHasProjectile{};
	transitionDead = new TransitionPlayerIsDead{};
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

	auto fontTiny = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);
	auto go = std::make_shared<dae::GameObject>();
	auto text = std::make_shared<dae::TextComponent>("Press WASD to move and SPACEBAR to shoot projectile to enemy or player", fontTiny);
	go->SetLocalPosition(0, 20);
	go->AddComponent(text);
	scene.Add(go);

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_shared<LevelComponent>());
	scene.Add(go);*/


	auto& widgetManager{ WidgetManager::GetInstance() };
	const auto levelSelectionWidget{std::make_shared<Widget>("LevelMenu")};
	auto newButton{ std::make_shared<ButtonComponent>("LoadLevel1Button",glm::vec3{100,100,0},"Level1",fontSmall) };
	newButton->SetOnButtonClick(DiggerUtils::OnLoadLevel1);
	levelSelectionWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("LoadLevel2Button",glm::vec3{100,120,0},"Level2",fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::OnLoadLevel2);
	levelSelectionWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("LoadLevel3Button",glm::vec3{100,140,0},"Level3",fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::OnLoadLevel3);
	levelSelectionWidget->AddElement(newButton);
	widgetManager.AddWidget(levelSelectionWidget);

	const auto mainMenuWidget{ std::make_shared<Widget>("MainMenu") };
	newButton = std::make_shared<ButtonComponent>("SinglePlayerButton", glm::vec3{100,100,0}, "SinglePlayer", fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::SelectSinglePlayer);
	mainMenuWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("CoopButton", glm::vec3{ 100,120,0 }, "Coop", fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::SelectCoop);
	mainMenuWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("VersusButton",glm::vec3{100,140,0},"Versus",fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::SelectVersus);
	mainMenuWidget->AddElement(newButton);
	widgetManager.AddWidget(mainMenuWidget);
	widgetManager.SetActiveWidget(mainMenuWidget);

	auto go{ std::make_shared<dae::GameObject>() };
	const auto gameStateComponent{std::make_shared<GameStateComponent>()};
	go->AddComponent(gameStateComponent);
	GameInstance::GetInstance().AddValue("GameState", go);
	DiggerUtils::DiggerGameMode gameMode{ DiggerUtils::DiggerGameMode::SinglePlayer };
	GameInstance::GetInstance().AddValue("CurrentGameMode", gameMode);
	int levelSelection{};
	GameInstance::GetInstance().AddValue("CurrentLevel",levelSelection);
	scene.Add(go);
}

int main()
{
    dae::Minigin engine{"../Data/" };
    engine.Run(load);
	return 0;
}
