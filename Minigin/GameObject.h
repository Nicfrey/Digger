#pragma once
#include <memory>
#include <vector>

#include "GameObjectComponent.h"
#include "Transform.h"

class GameObjectComponent;
class TextureManager;

namespace dae
{
	class Texture2D;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		void Init();
		void Update();
		void Render() const;

		template<typename T>
		bool AddComponent(std::shared_ptr<T> component);
		template<typename T>
		bool RemoveComponent(std::shared_ptr<T> component);
		bool RemoveComponentAtIndex(size_t index);
		template<typename T>
		std::shared_ptr<T> GetComponent() const;
		template<typename T>
		bool HasComponent() const;

		std::shared_ptr<GameObject> GetThis();
		void SetPosition(float x, float y);
		Transform GetTransform() const;

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		std::vector<std::shared_ptr<GameObjectComponent>> m_Components;
	};

	template <typename T>
	bool GameObject::AddComponent(std::shared_ptr<T> component)
	{
		if(std::shared_ptr<GameObjectComponent> componentCaster{ std::dynamic_pointer_cast<GameObjectComponent>(component) })
		{
			component->SetGameObject(GetThis());
			m_Components.emplace_back(component);
			return true;
		}
		return false;
	}

	template <typename T>
	bool GameObject::RemoveComponent(std::shared_ptr<T> component)
	{
		auto it{ std::find_if(m_Components.begin(),m_Components.end(), [component](const std::shared_ptr<GameObjectComponent> &other)
		{
				return component.get() == other.get();
		}) };
		if(it != m_Components.end())
		{
			m_Components.erase(it);
			return true;
		}
		return false;
	}

	template <typename T>
	std::shared_ptr<T> GameObject::GetComponent() const
	{
		for(std::shared_ptr<GameObjectComponent> component: m_Components)
		{
			std::shared_ptr<T> componentGot{ dynamic_cast<T>(component) };
			if(componentGot != nullptr)
			{
				return componentGot;
			}
		}
		return std::shared_ptr<T>{};
	}

	template <typename T>
	bool GameObject::HasComponent() const
	{
		for(auto& component: m_Components)
		{
			if(typeid(component) == typeid(T))
			{
				return true;
			}
		}
		return false;
	}
}
