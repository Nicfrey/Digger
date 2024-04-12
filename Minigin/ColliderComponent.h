#pragma once
#include "BaseComponent.h"

class ColliderComponent : public BaseComponent
{
public:
	ColliderComponent() = default;
	~ColliderComponent() override = default;
	ColliderComponent(const ColliderComponent& other) = delete;
	ColliderComponent(ColliderComponent&& other) noexcept = delete;
	ColliderComponent& operator=(const ColliderComponent& other) = delete;
	ColliderComponent& operator=(ColliderComponent&& other) noexcept = delete;

	virtual bool IsOverlapping(dae::GameObject* other);
	dae::GameObject* GetOther() const;
protected:
	void SetOther(dae::GameObject* other);
private:
	dae::GameObject* m_Other;
};

