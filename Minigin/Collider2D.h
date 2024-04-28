#pragma once
#include <glm/vec2.hpp>

#include "BaseComponent.h"
#include "ColliderComponent.h"

class Collider2D : public ColliderComponent
{
public:
	Collider2D() = default;
	std::shared_ptr<BaseComponent> Clone() const override;
	bool Intersect(const glm::vec3& p0, const glm::vec3& p1, glm::vec3& intersection, dae::GameObject*) const override = 0;

	bool IsOverlapping(std::shared_ptr<dae::GameObject>& other) override;
	bool IsRaycasting(std::shared_ptr<dae::GameObject>& other) override;
};

