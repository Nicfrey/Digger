#pragma once
#include <memory>
#include <vector>

#include "BaseComponent.h"
#include "Transform.h"

class BaseComponent;
class TextureComponent;

namespace dae
{
	class Texture2D;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		void Init();
		void Update();
		void FixedUpdate();
		void Render() const;
		void RenderGUI();


		template <typename T>
		bool AddComponent(std::shared_ptr<T> component);
		template <typename T>
		bool RemoveComponent(const std::shared_ptr<T>& component);
		bool RemoveComponentAtIndex(size_t index);
		template <typename T>
		std::shared_ptr<T> GetComponent() const;
		template <typename T>
		bool HasComponent() const;

		glm::vec3 GetWorldPosition();
		const glm::vec3& GetLocalPosition() const;
		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalPosition(const glm::vec2& pos);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(float x, float y);
		void Translate(const glm::vec3& pos);
		void Translate(const glm::vec2& pos);
		void Translate(float x, float y, float z);
		void Translate(float x, float y);
		std::shared_ptr<GameObject> GetThis();

		bool SetParent(const std::shared_ptr<GameObject>& newParent, bool keepWorldPosition = true);
		GameObject* GetParent() const;
		std::vector<GameObject*> GetChildren() const;


		GameObject() = default;
		GameObject(const glm::vec3& pos);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_LocalTransform{};
		Transform m_WorldTransform{};
		bool m_PositionIsDirty;
		std::vector<std::shared_ptr<BaseComponent>> m_Components;
		GameObject* m_ParentObject{};
		std::vector<GameObject*> m_ChildrenObject{};

		void SetPositionIsDirty();
		void UpdateWorldPosition();
		bool AddChild(const std::shared_ptr<GameObject>& child);
		bool RemoveChild(const std::shared_ptr<GameObject>& child);
		bool IsEqualToParent(const std::shared_ptr<GameObject>& child);
		bool IsChild(const std::shared_ptr<GameObject>& parent);
	};

	template <typename T>
	bool GameObject::AddComponent(std::shared_ptr<T> component)
	{
		if (std::shared_ptr<BaseComponent> componentCaster{std::dynamic_pointer_cast<BaseComponent>(component)})
		{
			component->SetGameObject(GetThis());
			m_Components.emplace_back(component);
			return true;
		}
		return false;
	}

	template <typename T>
	bool GameObject::RemoveComponent(const std::shared_ptr<T>& component)
	{
		auto it{
			std::find_if(m_Components.begin(), m_Components.end(),
			             [component](const std::shared_ptr<BaseComponent>& other)
			             {
				             return component.get() == other.get();
			             })
		};
		if (it != m_Components.end())
		{
			component->RemoveGameObject();
			m_Components.erase(it);
			return true;
		}
		return false;
	}

	template <typename T>
	std::shared_ptr<T> GameObject::GetComponent() const
	{
		for (std::shared_ptr<BaseComponent> component : m_Components)
		{
			std::shared_ptr<T> componentGot{std::dynamic_pointer_cast<T>(component)};
			if (componentGot != nullptr)
			{
				return componentGot;
			}
		}
		return std::shared_ptr<T>{};
	}

	template <typename T>
	bool GameObject::HasComponent() const
	{
		for (auto& component : m_Components)
		{
			if (typeid(component) == typeid(T))
			{
				return true;
			}
		}
		return false;
	}
}
