#include "Collider2D.h"

std::shared_ptr<BaseComponent> Collider2D::Clone() const
{
	return nullptr;
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

bool Collider2D::IsColliding(std::shared_ptr<dae::GameObject>& other)
{
	if(ColliderComponent::IsColliding(other))
	{
		return true;
	}
	SetOther(nullptr);
	return false;
}

bool Collider2D::IsRaycasting(std::shared_ptr<dae::GameObject>& other)
{
	if(ColliderComponent::IsRaycasting(other))
	{
		return true;
	}
	SetOther(nullptr);
	return false;
}
