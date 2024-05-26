#pragma once
#include "GameObject.h"
#include "SceneManager.h"

class SpatialGrid;
class QuadTree;

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Instantiate(std::shared_ptr<GameObject> object);
		void Remove();
		void Remove(std::shared_ptr<GameObject>& object);
		void RemoveAll();

		void Update();
		void Render() const;

		~Scene();
		void FixedUpdate();
		void Init();
		void RenderGUI();
		void OnCollisionUpdate();
		void UpdateQuadTree();
		std::string GetName() const { return m_name; }

		template<typename T>
		GameObject* GetGameObjectWithComponent() const;
		std::vector<GameObject*> GetGameObjectsWithComponent() const;
		GameObject* GetGameObjectByTag(const std::string& tag) const;
		std::vector<GameObject*> GetGameObjectsByTag(const std::string& tag) const;
		std::vector<std::shared_ptr<GameObject>> GetAllGameObject();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(std::string name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};
		std::vector < std::shared_ptr<GameObject>> m_ObjectsToBeAdded{};
		std::shared_ptr<QuadTree> m_QuadTree;
		std::shared_ptr<SpatialGrid> m_SpatialGrid;

		static unsigned int m_idCounter; 
	};

	template <typename T>
	GameObject* Scene::GetGameObjectWithComponent() const
	{
		for (const auto& object : m_objects)
		{
			if (object->HasComponent<T>())
			{
				return object.get();
			}
		}
		return nullptr;
	}
}
