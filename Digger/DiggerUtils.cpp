#include "DiggerUtils.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "Observer.h"
#include "Scene.h"
#include "SceneManager.h"

void DiggerUtils::OnLoadLevel1()
{
	dae::SceneManager::GetInstance().CreateScene("Level1");
	dae::SceneManager::GetInstance().SetActiveScene("Level1");
	const auto go{ std::make_shared<dae::GameObject>() };
	go->AddComponent(std::make_shared<LevelComponent>());
	dae::SceneManager::GetInstance().Instantiate(go);
	GameInstance::GetInstance().ChangeValue("CurrentLevel", 1);
	EventManager::GetInstance().NotifyEvent("SelectLevel");
}

void DiggerUtils::OnLoadLevel2()
{
	dae::SceneManager::GetInstance().CreateScene("Level2");
	dae::SceneManager::GetInstance().SetActiveScene("Level2");
	const auto go{ std::make_shared<dae::GameObject>() };
	go->AddComponent(std::make_shared<LevelComponent>());
	dae::SceneManager::GetInstance().Instantiate(go);
	GameInstance::GetInstance().ChangeValue("CurrentLevel", 2);
	EventManager::GetInstance().NotifyEvent("SelectLevel");
}

void DiggerUtils::OnLoadLevel3()
{
	dae::SceneManager::GetInstance().CreateScene("Level3");
	dae::SceneManager::GetInstance().SetActiveScene("Level3");
	const auto go{ std::make_shared<dae::GameObject>() };
	go->AddComponent(std::make_shared<LevelComponent>());
	dae::SceneManager::GetInstance().Instantiate(go);
	GameInstance::GetInstance().ChangeValue("CurrentLevel", 3);
	EventManager::GetInstance().NotifyEvent("SelectLevel");
}

void DiggerUtils::SkipLevel()
{
	int level;
	GameInstance::GetInstance().GetValue("CurrentLevel", level);
	dae::SceneManager::GetInstance().GetScene("Level"+std::to_string(level))->RemoveAll();
	++level;
	if(level > 3)
	{
		// TODO handle end of game
		return;
	}
	dae::SceneManager::GetInstance().CreateScene("Level" + std::to_string(level));
	dae::SceneManager::GetInstance().SetActiveScene("Level" + std::to_string(level));
	const auto go{ std::make_shared<dae::GameObject>() };
	go->AddComponent(std::make_shared<LevelComponent>());
	dae::SceneManager::GetInstance().Instantiate(go);
	GameInstance::GetInstance().ChangeValue("CurrentLevel", level);
	EventManager::GetInstance().NotifyEvent("LoadLevel");
}

void DiggerUtils::SelectSinglePlayer()
{
	EventManager::GetInstance().NotifyEvent("SelectGameMode");
	GameInstance::GetInstance().ChangeValue("CurrentGameMode", DiggerGameMode::SinglePlayer);
}

void DiggerUtils::SelectCoop()
{
	EventManager::GetInstance().NotifyEvent("SelectGameMode");
	GameInstance::GetInstance().ChangeValue("CurrentGameMode", DiggerGameMode::Coop);
}

void DiggerUtils::SelectVersus()
{
	EventManager::GetInstance().NotifyEvent("SelectGameMode");
	GameInstance::GetInstance().ChangeValue("CurrentGameMode", DiggerGameMode::Versus);
}

void DiggerUtils::HasSetName()
{
	EventManager::GetInstance().NotifyEvent("SetNameHighScore");
}
