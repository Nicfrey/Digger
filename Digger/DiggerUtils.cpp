#include "DiggerUtils.h"

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
	EventManager::GetInstance().NotifyEvent("LoadLevel1");
}

void DiggerUtils::OnLoadLevel2()
{
	auto& scene{ dae::SceneManager::GetInstance().CreateScene("Level2") };
	const auto go{ std::make_shared<dae::GameObject>() };
	go->AddComponent(std::make_shared<LevelComponent>());
	scene.Add(go);
	EventManager::GetInstance().NotifyEvent("LoadLevel2");
}

void DiggerUtils::OnLoadLevel3()
{
	auto& scene{ dae::SceneManager::GetInstance().CreateScene("Level3") };
	const auto go{ std::make_shared<dae::GameObject>() };
	go->AddComponent(std::make_shared<LevelComponent>());
	scene.Add(go);
	EventManager::GetInstance().NotifyEvent("LoadLevel3");
}
