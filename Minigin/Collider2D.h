#pragma once
#include "BaseComponent.h"
#include "ColliderComponent.h"

class Collider2D : public ColliderComponent
{
public:
	Collider2D() = default;
	~Collider2D() override = default;
	Collider2D(const Collider2D& other) = delete;
	Collider2D(Collider2D&& other) noexcept = delete;
	Collider2D& operator=(const Collider2D& other) = delete;
	Collider2D& operator=(Collider2D&& other) noexcept = delete;

	bool IsOverlapping(dae::GameObject* other) override;
};

