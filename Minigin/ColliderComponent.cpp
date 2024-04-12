#include "ColliderComponent.h"

#include "GameObject.h"

bool ColliderComponent::IsOverlapping(dae::GameObject* other)
{
	if (!other)
	{
		SetOther(nullptr);
		return false;
	}
	return other->GetComponent<ColliderComponent>() != nullptr;
}

dae::GameObject* ColliderComponent::GetOther() const
{
	return m_Other;
}

void ColliderComponent::SetOther(dae::GameObject* other)
{
	m_Other = other;
}
