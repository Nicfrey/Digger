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

	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void Init() {}
	virtual void Render() const {}
	virtual void RenderGUI() {}
	virtual void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) { if (!other) return; }
	virtual void OnCollisionExit(std::shared_ptr<dae::GameObject>& other) { if (!other) return; }
	virtual void OnCollisionStay(std::shared_ptr<dae::GameObject>& other) { if (!other) return; }
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

