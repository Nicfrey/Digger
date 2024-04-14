#pragma once
#include <memory>
#include <string>
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
		void SetTag(const std::string& tag);
		std::string GetTag() const;


		template <typename T>
		bool AddComponent(std::shared_ptr<T> component);
		template <typename T>
		bool RemoveComponent(const std::shared_ptr<T>& component);
		bool RemoveComponentAtIndex(size_t index);
		template <typename T>
		std::shared_ptr<T> GetComponent() const;
		template <typename T>
		bool HasComponent() const;

		// Positions
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

		glm::vec3 GetWorldRotation();
		const glm::vec3& GetLocalRotation() const;
		void SetLocalRotation(const glm::vec3& rot);
		void SetLocalRotation(const glm::vec2& rot);
		void SetLocalRotation(float x, float y, float z);
		void SetLocalRotation(float x, float y);
		void OnCollisionEnter(std::shared_ptr<GameObject>& other);
		void OnCollisionExit(std::shared_ptr<GameObject>& other);
		void OnCollisionStay(std::shared_ptr<GameObject>& other);
		std::shared_ptr<GameObject> GetThis();

		bool SetParent(const std::shared_ptr<GameObject>& newParent, bool keepWorldPosition = true);
		GameObject* GetParent() const;
		std::vector<GameObject*> GetChildren() const;
		glm::vec3 GetForward() const;


		GameObject() = default;
		GameObject(const glm::vec3& pos);
		~GameObject();
		GameObject(const GameObject& other);
		GameObject(GameObject&& other) noexcept;
		GameObject& operator=(const GameObject& other);
		GameObject& operator=(GameObject&& other) noexcept;

	private:
		Transform m_LocalTransform{};
		Transform m_WorldTransform{};
		bool m_PositionIsDirty;
		bool m_RotationIsDirty;
		std::vector<std::shared_ptr<BaseComponent>> m_Components;
		GameObject* m_ParentObject{};
		std::vector<GameObject*> m_ChildrenObject{};

		std::string m_Tag{};

		void SetPositionIsDirty();
		void UpdateWorldPosition();
		void SetRotationIsDirty();
		void UpdateWorldRotation();
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
		for (const std::shared_ptr<BaseComponent>& component : m_Components)
		{
			std::shared_ptr<T> componentGot{std::dynamic_pointer_cast<T>(component)};
			if (componentGot != nullptr)
			{
				return componentGot;
			}
		}
		return std::shared_ptr<T>{nullptr};
	}

	template <typename T>
	bool GameObject::HasComponent() const
	{
		return GetComponent<T>() != nullptr;
	}
}
