#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::RenderGUI()
{
	for (const auto& scene : m_scenes)
	{
		scene->RenderGUI();
	}
}

void dae::SceneManager::FixedUpdate()
{
	for (const auto& scene : m_scenes)
	{
		scene->FixedUpdate();
	}
}

void dae::SceneManager::OnCollisionUpdate()
{
	for(const auto& scene : m_scenes)
	{
		scene->OnCollisionUpdate();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::Init()
{
	for (const auto& scene : m_scenes)
	{
		scene->Init();
	}
}
