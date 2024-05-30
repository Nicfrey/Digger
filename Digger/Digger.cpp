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
#include "TextComponent.h"
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
	ss.Add(static_cast<SoundId>(DiggerUtils::SoundDiggerID::PROJECTILE_HIT), "Sounds/ProjectileHitWall.wav");
	ms.Add(static_cast<MusicId>(DiggerUtils::MusicDiggerID::GAME),"Sounds/DiggerMusic.mp3");
	ms.Add(static_cast<MusicId>(DiggerUtils::MusicDiggerID::PLAYER_DIED), "Sounds/GameOverMusic.mp3");
	ms.Add(static_cast<MusicId>(DiggerUtils::MusicDiggerID::MAIN_MENU), "Sounds/MainMenuMusic.mp3");
	ms.Add(static_cast<MusicId>(DiggerUtils::MusicDiggerID::WIN),"Sounds/LevelCompleteMusic.mp3");
	auto& scene = dae::SceneManager::GetInstance().CreateScene("MenuDigger");
	dae::SceneManager::GetInstance().SetActiveScene("MenuDigger");

	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("DiggerFont.ttf", 40);

	Rectf placeholderTitle{ glm::vec2{0,100},dae::Minigin::m_Window.x, 100.f };
	const auto titleDigger{ std::make_shared<dae::TextComponent>("DIGGER",fontSmall) };
	titleDigger->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(Rectf{ glm::vec2{},static_cast<float>(titleDigger->GetSize().x),static_cast<float>(titleDigger->GetSize().y) }, placeholderTitle));

	auto& widgetManager{ WidgetManager::GetInstance() };
	const auto levelSelectionWidget{std::make_shared<Widget>("LevelMenu")};
	levelSelectionWidget->AddElement(titleDigger);

	placeholderTitle.topLeft.y += 100;
	auto newButton{ std::make_shared<ButtonComponent>("LoadLevel1Button",glm::vec3{100,100,0},"Level1",fontSmall) };
	newButton->SetOnButtonClick(DiggerUtils::OnLoadLevel1);
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetSize(), placeholderTitle));
	levelSelectionWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("LoadLevel2Button",glm::vec3{100,120,0},"Level2",fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::OnLoadLevel2);
	placeholderTitle.topLeft.y += 60;
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetSize(), placeholderTitle));
	levelSelectionWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("LoadLevel3Button",glm::vec3{100,140,0},"Level3",fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::OnLoadLevel3);
	placeholderTitle.topLeft.y += 60;
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetSize(), placeholderTitle));
	levelSelectionWidget->AddElement(newButton);
	widgetManager.AddWidget(levelSelectionWidget);

	placeholderTitle.topLeft.y = 100;
	const auto mainMenuWidget{ std::make_shared<Widget>("MainMenu") };
	mainMenuWidget->AddElement(titleDigger);
	placeholderTitle.topLeft.y += 100;
	newButton = std::make_shared<ButtonComponent>("SinglePlayerButton", glm::vec3{100,100,0}, "SinglePlayer", fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::SelectSinglePlayer);
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetSize(), placeholderTitle));
	mainMenuWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("CoopButton", glm::vec3{ 100,120,0 }, "Coop", fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::SelectCoop);
	placeholderTitle.topLeft.y += 60;
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetSize(), placeholderTitle));
	mainMenuWidget->AddElement(newButton);
	newButton = std::make_shared<ButtonComponent>("VersusButton",glm::vec3{100,140,0},"Versus",fontSmall);
	newButton->SetOnButtonClick(DiggerUtils::SelectVersus);
	placeholderTitle.topLeft.y += 60;
	newButton->SetPositionOffset(Utils::GetPositionForRectangleToBeCentered(newButton->GetSize(), placeholderTitle));
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
	ms.Play(static_cast<MusicId>(DiggerUtils::MusicDiggerID::MAIN_MENU), true);
}

int main()
{
    dae::Minigin engine{"../Data/" };
    engine.Run(load);
	return 0;
}
