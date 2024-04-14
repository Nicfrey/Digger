#include "Collider2D.h"

bool Collider2D::IsOverlapping(std::shared_ptr<dae::GameObject>& other)
{
	if(ColliderComponent::IsOverlapping(other))
	{
		return true;
	}
	SetOther(nullptr);
	return false;
}
