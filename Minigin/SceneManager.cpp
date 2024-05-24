#include "SceneManager.h"

#include <iostream>
#include <thread>
#include <utility>

#include "Scene.h"

void dae::SceneManager::Update()
{
	m_ActiveScene->Update();
}

void dae::SceneManager::Render()
{
	m_ActiveScene->Render();
}

void dae::SceneManager::RenderGUI()
{
	m_ActiveScene->RenderGUI();
}

void dae::SceneManager::FixedUpdate()
{
	m_ActiveScene->FixedUpdate();
}

void dae::SceneManager::OnCollisionUpdate()
{
	std::jthread thread([this]
		{
			m_MutexCollision.lock();
			m_ActiveScene->OnCollisionUpdate();
			m_MutexCollision.unlock();
		});
	// m_ActiveScene->OnCollisionUpdate();
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	const auto it{ std::ranges::find_if(m_scenes,[name](const std::shared_ptr<Scene>& other)
	{
		return other->GetName() == name;
	}) };
	if(it != m_scenes.end())
	{
		m_ActiveScene = *it;
	}
	else
	{
		std::cerr << "Scene with name: " << name << " not found\n";
	}
}

void dae::SceneManager::SetActiveScene(std::shared_ptr<Scene> scene)
{
	const auto it{ std::ranges::find(m_scenes, scene) };
	if(it != m_scenes.end())
	{
		m_ActiveScene = *it;
	}
	else
	{
		std::cerr << "Scene not found\n";
	}
}

void dae::SceneManager::Instantiate(std::shared_ptr<GameObject> object)
{
	m_ActiveScene->Instantiate(std::move(object));
}

dae::GameObject* dae::SceneManager::GetGameObjectByTag(const std::string& tag) const
{
	const auto go = m_ActiveScene->GetGameObjectByTag(tag);
	if (go != nullptr)
	{
		return go;
	}
	return nullptr;
}

std::vector<dae::GameObject*> dae::SceneManager::GetGameObjectsByTag(const std::string& tag) const
{
	std::vector<GameObject*> gameObjects;

	const auto go = m_ActiveScene->GetGameObjectsByTag(tag);
	if (!go.empty())
	{
		gameObjects.insert(gameObjects.end(), go.begin(), go.end());
	}
	return gameObjects;
}

std::vector<std::shared_ptr<dae::GameObject>> dae::SceneManager::GetAllGameObject() const
{
	return m_ActiveScene->GetAllGameObject();
}

void dae::SceneManager::OnDestroy()
{
	m_ActiveScene->Remove();
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetScene(const std::string& name)
{
	const auto it{ std::ranges::find_if(m_scenes,[name](const std::shared_ptr<Scene>& other)
	{
		return other->GetName() == name;
	}) };
	if (it != m_scenes.end())
	{
		return *it;
	}
	std::cerr << "Scene with name: " << name << " not found\n";
	return nullptr;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::Init()
{
	m_ActiveScene->Init();
}
