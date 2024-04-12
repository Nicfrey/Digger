#include "Collider2D.h"

bool Collider2D::IsOverlapping(dae::GameObject* other)
{
	if(ColliderComponent::IsOverlapping(other))
	{
		return true;
	}
	SetOther(nullptr);
	return false;
}
