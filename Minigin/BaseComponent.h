#pragma once
#include <memory>

#include "Singleton.h"

namespace dae
{
	class GameObject;
}

class BaseComponent
{
public:
	BaseComponent() = default;
	BaseComponent(dae::GameObject* pGameObject): m_GameObject{pGameObject} {}
	virtual ~BaseComponent() = default;
	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) noexcept = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) noexcept = delete;

	virtual void Update() = 0;
	virtual void FixedUpdate() = 0;
	virtual void Init() = 0;
	virtual void Render() const = 0;
	void SetGameObject(const std::shared_ptr<dae::GameObject>& go);
protected:
	dae::GameObject* GetGameObject() const
	{
		return m_GameObject;
	}
private:
	dae::GameObject* m_GameObject;
};

