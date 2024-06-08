#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include <windows.h>
#include <iostream>
#include <Xinput.h>

#include "ButtonComponent.h"
#include "Controller.h"
#include "DiggerCommands.h"
#include "DiggerStates.h"
#include "DiggerTransitions.h"
#include "DiggerUtils.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "GameState.h"
#include "InputManager.h"
#include "KeyboardComponent.h"
#include "Minigin.h"
#include "Observer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SoundSystemEngine.h"
#include "TextComponent.h"
#include "Widget.h"
#include "WidgetManager.h"

void InitGameStates()
{
	auto& gameState{ GameState::GetInstance() };
	const auto menuState{ new MenuState{} };
	const auto loadState{ new LoadState{} };
	const auto playState{ new PlayState{} };
	const auto gameOverState{ new GameOverState() };
	const auto respawnState{ new RespawnState() };
	const auto scoreState{ new ScoreState() };
	gameState.AddState(menuState);
	gameState.AddState(loadState);
	gameState.AddState(playState);
	gameState.AddState(gameOverState);
	gameState.AddState(respawnState);
	gameState.AddState(scoreState);
	const auto selectedLevelTrans{ new HasSelectedLevelTransition{} };
	const auto loadLevelTransition{ new LoadLevelTransition{} };
	const auto playerDeadTransition{ new PlayerDeadTransition{} };
	const auto respawnTransition{ new PlayerRespawnTransition{} };
	const auto gameOverTransition{ new GameOverTransition{} };
	const auto wroteHighScoreTransition{ new WroteHighScoreTransition{} };
	const auto skipLevelTransition{ new SkipLevelTransition{} };
	const auto playerWinTransition{ new PlayerWinTransition{} };
	const auto returnToMainMenuTransition{ new ReturnToMainMenuTransition{} };
	gameState.AddTransition(menuState, loadState, selectedLevelTrans);
	gameState.AddTransition(loadState, playState, loadLevelTransition);
	gameState.AddTransition(playState, respawnState, playerDeadTransition);
	gameState.AddTransition(respawnState, playState, respawnTransition);
	gameState.AddTransition(playState, gameOverState, gameOverTransition);
	gameState.AddTransition(gameOverState, scoreState, wroteHighScoreTransition);
	gameState.AddTransition(scoreState, menuState, returnToMainMenuTransition);
	gameState.AddTransition(playState, loadState, skipLevelTransition);
	gameState.AddTransition(playState, loadState, playerWinTransition);
	gameState.AddParameter("hasSelectedLevel", false);
	gameState.AddParameter("hasSelectedGameMode", false);
	gameState.AddParameter("hasLoadedLevel", false);
	gameState.AddParameter("currentLevel", 0);
	gameState.AddParameter("hasLoadedLevel", false);
	gameState.AddParameter("isPlayerDead", false);
	gameState.AddParameter("hasWrittenHighScore", false);
	gameState.AddParameter("hasNoExtraLife", false);
	gameState.AddParameter("levelRemaining", 2);
	gameState.AddParameter("hasSkippedLevel", false);
	gameState.AddParameter("hasPlayerWon", false);
	gameState.AddParameter("ReturnToMainMenu", false);
	// gameState.AddParameter("players", dae::SceneManager::GetInstance().GetGameObjectsWithComponent<PlayerComponent>());
	gameState.SetStartState(menuState);
}

void InitCommandUI()
{

	GamepadController* controller{ new GamepadController{0} };
	const auto selectButtonCommand{ std::make_shared<SelectButtonCommand>() };
	const auto moveUpButtonCommand{ std::make_shared<MoveButtonCommand>(true) };
	const auto moveDownButtonCommand{ std::make_shared<MoveButtonCommand>(false) };
	const auto moveUpKeyboardCommand{ std::make_shared<MoveKeyboardCommand>(glm::ivec2{0,1}) };
	const auto moveDownKeyboardCommand{ std::make_shared<MoveKeyboardCommand>(glm::ivec2{0,-1}) };
	const auto moveRightKeyboardCommand{ std::make_shared<MoveKeyboardCommand>(glm::ivec2{1,0}) };
	const auto moveLeftKeyboardCommand{ std::make_shared<MoveKeyboardCommand>(glm::ivec2{-1,0}) };
	const auto saveKeyboardCommand{ std::make_shared<SaveKeyboardCommand>() };
	controller->BindAction(selectButtonCommand, XINPUT_GAMEPAD_A, TriggerType::KeyPressed);
	controller->BindAction(moveUpButtonCommand, XINPUT_GAMEPAD_DPAD_UP, TriggerType::KeyPressed);
	controller->BindAction(moveDownButtonCommand, XINPUT_GAMEPAD_DPAD_DOWN, TriggerType::KeyPressed);
	controller->BindAction(moveUpKeyboardCommand, XINPUT_GAMEPAD_DPAD_DOWN, TriggerType::KeyPressed);
	controller->BindAction(moveDownKeyboardCommand, XINPUT_GAMEPAD_DPAD_UP, TriggerType::KeyPressed);
	controller->BindAction(moveRightKeyboardCommand, XINPUT_GAMEPAD_DPAD_RIGHT, TriggerType::KeyPressed);
	controller->BindAction(moveLeftKeyboardCommand, XINPUT_GAMEPAD_DPAD_LEFT, TriggerType::KeyPressed);
	controller->BindAction(saveKeyboardCommand, XINPUT_GAMEPAD_START, TriggerType::KeyPressed);
	dae::InputManager::GetInstance().AddController(controller);
	dae::InputManager::GetInstance().BindCommand(std::make_shared<SaveKeyboardCommand>(), SDL_SCANCODE_SPACE, TriggerType::KeyPressed);

	const auto skipLevelCommand{ std::make_shared<SkipLevelCommand>() };
	dae::InputManager::GetInstance().BindCommand(skipLevelCommand, SDL_SCANCODE_F1, TriggerType::KeyPressed);
}

void InitGameInstance()
{
	GameInstance::GetInstance().AddValue("CurrentGameMode", DiggerUtils::DiggerGameMode::SinglePlayer);
	GameInstance::GetInstance().AddValue("CurrentLevel", 0);
	GameInstance::GetInstance().AddValue("Score", 0);
	GameInstance::GetInstance().AddValue("Lives", 3);
}

void InitServicesLocators()
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
	ss.Add(TO_SOUND_ID(DiggerUtils::SoundDiggerID::PROJECTILE_HIT_ENTITIES), "Sounds/ProjectileHitEntities.wav");
	ss.Add(TO_SOUND_ID(DiggerUtils::SoundDiggerID::PROJECTILE_HIT_WALL), "Sounds/ProjectileHitWall.wav");
	ss.Add(TO_SOUND_ID(DiggerUtils::SoundDiggerID::COLLECT_EMERALD), "Sounds/EmeraldCollect.wav");
	ss.Add(TO_SOUND_ID(DiggerUtils::SoundDiggerID::COLLECT_MONEY_BAG), "Sounds/MoneyBagCollected.wav");
	ss.Add(TO_SOUND_ID(DiggerUtils::SoundDiggerID::PROJECTILE_SHOOT), "Sounds/ShootProjectile.wav");
	ms.Add(TO_MUSIC_ID(DiggerUtils::MusicDiggerID::GAME), "Sounds/DiggerMusic.mp3");
	ms.Add(TO_MUSIC_ID(DiggerUtils::MusicDiggerID::PLAYER_DIED), "Sounds/GameOverMusic.mp3");
	ms.Add(TO_MUSIC_ID(DiggerUtils::MusicDiggerID::MAIN_MENU), "Sounds/MainMenuMusic.mp3");
	ms.Add(TO_MUSIC_ID(DiggerUtils::MusicDiggerID::WIN), "Sounds/LevelCompleteMusic.mp3");
	ms.Add(TO_MUSIC_ID(DiggerUtils::MusicDiggerID::GAME_OVER), "Sounds/GameOverMenuMusic.mp3");
}

void InitWidgets()
{
	dae::SceneManager::GetInstance().CreateScene("MenuDigger");
	dae::SceneManager::GetInstance().SetActiveScene("MenuDigger");

	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 40);
	auto fontBig = dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 60);

	Rectf placeholderTitle{ glm::vec2{0,100},dae::Minigin::m_Window.x, 100.f };
	const auto titleDigger{ std::make_shared<dae::TextComponent>("DIGGER",fontBig) };
	titleDigger->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(Rectf{ static_cast<float>(titleDigger->GetSize().x),static_cast<float>(titleDigger->GetSize().y) }, placeholderTitle));

	auto& widgetManager{ WidgetManager::GetInstance() };
	const auto levelSelectionWidget{ std::make_shared<Widget>("LevelMenu") };
	levelSelectionWidget->AddElement(titleDigger);

	placeholderTitle.topLeft.y += 100;
	auto newButton{ std::make_shared<ButtonComponent>("LoadLevel1Button",glm::vec3{100,100,0},"Level1",fontSmall) };
	newButton->SetOnButtonClick(DiggerUtils::OnLoadLevel1);
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetBox(), placeholderTitle));
	levelSelectionWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("LoadLevel2Button", glm::vec3{ 100,120,0 }, "Level2", fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::OnLoadLevel2);
	placeholderTitle.topLeft.y += 60;
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetBox(), placeholderTitle));
	levelSelectionWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("LoadLevel3Button", glm::vec3{ 100,140,0 }, "Level3", fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::OnLoadLevel3);
	placeholderTitle.topLeft.y += 60;
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetBox(), placeholderTitle));
	levelSelectionWidget->AddElement(newButton);
	widgetManager.AddWidget(levelSelectionWidget);

	placeholderTitle.topLeft.y = 100;
	const auto mainMenuWidget{ std::make_shared<Widget>("MainMenu") };
	mainMenuWidget->AddElement(titleDigger);

	placeholderTitle.topLeft.y += 100;
	newButton = std::make_shared<ButtonComponent>("SinglePlayerButton", glm::vec3{ 100,100,0 }, "SinglePlayer", fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::SelectSinglePlayer);
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetBox(), placeholderTitle));
	mainMenuWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("CoopButton", glm::vec3{ 100,120,0 }, "Coop", fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::SelectCoop);
	placeholderTitle.topLeft.y += 60;
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetBox(), placeholderTitle));
	mainMenuWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("VersusButton", glm::vec3{ 100,140,0 }, "Versus", fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::SelectVersus);
	placeholderTitle.topLeft.y += 60;
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetBox(), placeholderTitle));
	mainMenuWidget->AddElement(newButton);
	widgetManager.AddWidget(mainMenuWidget);
	widgetManager.SetActiveWidget(mainMenuWidget);

	placeholderTitle.topLeft.y = 10;
	const auto gameOverWidget{ std::make_shared<Widget>("GameOver") };
	const auto titleGameOver{ std::make_shared<dae::TextComponent>("Game Over",fontBig) };
	titleGameOver->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(Rectf{ static_cast<float>(titleGameOver->GetSize().x), static_cast<float>(titleGameOver->GetSize().y) }, placeholderTitle));
	const auto keyboard = std::make_shared<KeyboardComponent>("Keyboard", 200.f);
	placeholderTitle.topLeft.y += 60;
	const auto totalScore = std::make_shared<dae::TextComponent>("TotalScore", fontSmall);
	totalScore->SetColor(255, 0, 0);
	totalScore->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(Rectf{ static_cast<float>(totalScore->GetSize().x), static_cast<float>(totalScore->GetSize().y) }, placeholderTitle));
	placeholderTitle.topLeft.y += 60;
	const auto name = std::make_shared<dae::TextComponent>("Enter Name", fontSmall);
	name->SetColor(255, 255, 0);
	name->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(Rectf{ static_cast<float>(name->GetSize().x), static_cast<float>(name->GetSize().y) }, placeholderTitle));
	gameOverWidget->AddElement(keyboard);
	gameOverWidget->AddElement(titleGameOver);
	gameOverWidget->AddElement(name);
	gameOverWidget->AddElement(totalScore);
	widgetManager.AddWidget(gameOverWidget);

	placeholderTitle.topLeft.y = 10;
	const auto scoreWidget{ std::make_shared<Widget>("DisplayScore") };
	const auto titleScore{ std::make_shared<dae::TextComponent>("Score",fontBig) };
	titleScore->SetColor(255, 0, 0);
	titleScore->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(Rectf{ static_cast<float>(titleScore->GetSize().x), static_cast<float>(titleScore->GetSize().y) }, placeholderTitle));
	placeholderTitle.topLeft.y += 80;
	for (int i{ 1 }; i <= 5; ++i)
	{
		const auto score = std::make_shared<dae::TextComponent>("HighScore" + std::to_string(i), fontSmall);
		score->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(Rectf{ static_cast<float>(score->GetSize().x), static_cast<float>(score->GetSize().y) }, placeholderTitle));
		score->SetColor(255, 0, 0);
		scoreWidget->AddElement(score);
		placeholderTitle.topLeft.y += 50;
	}
	const auto mainMenuButton{ std::make_shared<ButtonComponent>("MainMenuButton",glm::vec3{100.f,300.f,0.f },"Return to menu",fontSmall) };
	mainMenuButton->SetOnButtonClick(DiggerUtils::ReturnToMenu);
	mainMenuButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(mainMenuButton->GetBox(), placeholderTitle));
	scoreWidget->AddElement(titleScore);
	scoreWidget->AddElement(mainMenuButton);
	widgetManager.AddWidget(scoreWidget);

	const auto gameUI{ std::make_shared<Widget>("GameUI") };
	placeholderTitle.topLeft.y = dae::Minigin::m_Window.y - 50;
	const auto scoreText{ std::make_shared<dae::TextComponent>("Score",fontSmall) };
	scoreText->SetPositionOffset(glm::vec2{ 0.f,dae::Minigin::m_Window.y - 50.f });
	const auto lifeText{ std::make_shared<dae::TextComponent>("Life",fontSmall) };
	scoreText->SetPositionOffset(glm::vec2{ 0.f,dae::Minigin::m_Window.y - 50.f });
	lifeText->SetPositionOffset(glm::vec2{ 200,dae::Minigin::m_Window.y - 50.f });
	gameUI->AddElement(scoreText);
	gameUI->AddElement(lifeText);
	widgetManager.AddWidget(gameUI);
}

void load()
{
	InitServicesLocators();
	InitWidgets();
	InitCommandUI();
	InitGameStates();
	InitGameInstance();
	EventManager::GetInstance().AddEvent("NextLevel", DiggerUtils::NextLevel);
	EventManager::GetInstance().AddEvent("GameOver", DiggerUtils::LoadGameOver);
}

int main()
{
    dae::Minigin engine{"../Data/" };
    engine.Run(load);
	return 0;
}
