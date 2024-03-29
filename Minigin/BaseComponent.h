#pragma once
#include <memory>

namespace dae
{
	class GameObject;
}

class BaseComponent : public::std::enable_shared_from_this<BaseComponent>
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
	virtual void RenderGUI() = 0;
	void SetGameObject(const std::shared_ptr<dae::GameObject>& go);
	void RemoveGameObject();
protected:
	dae::GameObject* GetGameObject() const
	{
		return m_GameObject;
	}
	std::shared_ptr<BaseComponent> GetThis();
private:
	dae::GameObject* m_GameObject;
};

