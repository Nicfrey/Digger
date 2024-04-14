#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class GameObject;
}

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Init();
		void Update();
		void Render();
		void RenderGUI();
		void FixedUpdate();
		void OnCollisionUpdate();
		void SetActiveScene(const std::string& name);
		void SetActiveScene(std::shared_ptr<Scene> scene);
		void Instantiate(std::shared_ptr<GameObject> object);
		template<typename T>
		GameObject* GetGameObjectWithComponent();
		template<typename T>
		std::vector<GameObject*> GetGameObjectsWithComponent();
		GameObject* GetGameObjectByTag(const std::string& tag) const;
		std::vector<GameObject*> GetGameObjectsByTag(const std::string& tag) const;
		void Destroy();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_ActiveScene;
	};

	template <typename T>
	GameObject* SceneManager::GetGameObjectWithComponent()
	{
		for (const auto& scene : m_scenes)
		{
			auto gameObject = scene->GetGameObjectWithComponent<T>();
			if (gameObject != nullptr)
				return gameObject;
		}
		return nullptr;
	}

	template <typename T>
	std::vector<GameObject*> SceneManager::GetGameObjectsWithComponent()
	{
		std::vector<GameObject*> gameObjects;
		for (const auto& scene : m_scenes)
		{
			const auto go = scene->GetGameObjectsWithComponent<T>();
			if(!go.empty())
			{
				gameObjects.insert(gameObjects.end(), go.begin(), go.end());
			}
		}
		return gameObjects;
	}
}
