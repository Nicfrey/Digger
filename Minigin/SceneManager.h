#pragma once
#include <vector>
#include <string>
#include <memory>
#include <mutex>

#include "Singleton.h"

namespace dae
{
	class Scene;
	class GameObject;
}

namespace dae
{
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
		std::shared_ptr<GameObject> GetGameObjectWithComponent();
		template<typename T>
		std::vector<std::shared_ptr<GameObject>> GetGameObjectsWithComponent();
		std::shared_ptr<GameObject> GetGameObjectByTag(const std::string& tag) const;
		std::vector<std::shared_ptr<GameObject>> GetGameObjectsByTag(const std::string& tag) const;
		std::vector<std::shared_ptr<GameObject>> GetAllGameObject() const;
		void OnDestroy();

		std::shared_ptr<dae::Scene> GetScene(const std::string& name);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_ActiveScene;

	};

	template <typename T>
	std::shared_ptr<GameObject> SceneManager::GetGameObjectWithComponent()
	{
		for (const auto& scene : m_scenes)
		{
			if (m_ActiveScene == scene)
			{
				auto gameObject = scene->GetGameObjectWithComponent<T>();
				if (gameObject != nullptr)
					return gameObject;
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<std::shared_ptr<GameObject>> SceneManager::GetGameObjectsWithComponent()
	{
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		for (const auto& scene : m_scenes)
		{
			if(m_ActiveScene == scene)
			{
				const auto go = scene->GetGameObjectsWithComponent<T>();
				if (!go.empty())
				{
					gameObjects.insert(gameObjects.end(), go.begin(), go.end());
				}
			}
		}
		return gameObjects;
	}
}
