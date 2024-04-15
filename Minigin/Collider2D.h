#pragma once
#include "BaseComponent.h"
#include "ColliderComponent.h"

class Collider2D : public ColliderComponent
{
public:
	Collider2D() = default;
	~Collider2D() override = default;
	Collider2D(const Collider2D& other) = default;
	Collider2D(Collider2D&& other) noexcept;
	Collider2D& operator=(const Collider2D& other);
	Collider2D& operator=(Collider2D&& other) noexcept;
	std::shared_ptr<BaseComponent> Clone() const override;

	bool IsOverlapping(std::shared_ptr<dae::GameObject>& other) override;
};

