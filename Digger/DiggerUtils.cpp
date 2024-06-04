#include "DiggerUtils.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "Observer.h"
#include "Scene.h"
#include "SceneManager.h"

void DiggerUtils::OnLoadLevel1()
{
	auto& scene{ dae::SceneManager::GetInstance().CreateScene("Level1") };
	const auto go{ std::make_shared<dae::GameObject>() };
	go->AddComponent(std::make_shared<LevelComponent>());
	scene.Add(go);
	GameInstance::GetInstance().ChangeValue("CurrentLevel", 1);
	EventManager::GetInstance().NotifyEvent("SelectLevel");
}

void DiggerUtils::OnLoadLevel2()
{
	auto& scene{ dae::SceneManager::GetInstance().CreateScene("Level2") };
	const auto go{ std::make_shared<dae::GameObject>() };
	go->AddComponent(std::make_shared<LevelComponent>());
	scene.Add(go);
	GameInstance::GetInstance().ChangeValue("CurrentLevel", 2);
	EventManager::GetInstance().NotifyEvent("SelectLevel");
}

void DiggerUtils::OnLoadLevel3()
{
	auto& scene{ dae::SceneManager::GetInstance().CreateScene("Level3") };
	const auto go{ std::make_shared<dae::GameObject>() };
	go->AddComponent(std::make_shared<LevelComponent>());
	scene.Add(go);
	GameInstance::GetInstance().ChangeValue("CurrentLevel", 3);
	EventManager::GetInstance().NotifyEvent("SelectLevel");
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
