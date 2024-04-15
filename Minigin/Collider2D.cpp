#include "Collider2D.h"

Collider2D::Collider2D(Collider2D&& other) noexcept: ColliderComponent(std::move(other))
{
}

Collider2D& Collider2D::operator=(const Collider2D& other)
{
	if (this == &other)
	{
		return *this;
	}
	ColliderComponent::operator=(other);
	return *this;
}

Collider2D& Collider2D::operator=(Collider2D&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	ColliderComponent::operator=(std::move(other));
	return *this;
}

std::shared_ptr<BaseComponent> Collider2D::Clone() const
{
	return std::make_shared<Collider2D>(*this);
}

bool Collider2D::IsOverlapping(std::shared_ptr<dae::GameObject>& other)
{
	if(ColliderComponent::IsOverlapping(other))
	{
		return true;
	}
	SetOther(nullptr);
	return false;
}
