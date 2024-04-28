#pragma once
#include <glm/vec3.hpp>

#include "BaseComponent.h"

class ColliderComponent : public BaseComponent
{
public:
	ColliderComponent() = default;
	~ColliderComponent() override = default;
	ColliderComponent(const ColliderComponent& other);
	ColliderComponent(ColliderComponent&& other) noexcept;
	ColliderComponent& operator=(const ColliderComponent& other);
	ColliderComponent& operator=(ColliderComponent&& other) noexcept;
	std::shared_ptr<BaseComponent> Clone() const override;

	virtual bool IsOverlapping(std::shared_ptr<dae::GameObject>& other);
	virtual bool Intersect(const glm::vec3& p0, const glm::vec3& p1, glm::vec3& intersection, dae::GameObject* go) const = 0;
	virtual bool IsRaycasting(std::shared_ptr<dae::GameObject>& other);
	dae::GameObject* GetOther() const;
protected:
	void SetOther(dae::GameObject* other);
private:
	dae::GameObject* m_Other;
};

