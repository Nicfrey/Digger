#include "ColliderComponent.h"

#include "GameObject.h"

ColliderComponent::ColliderComponent(const ColliderComponent& other): ColliderComponent{}
{
	m_Other = other.m_Other;
}

ColliderComponent::ColliderComponent(ColliderComponent&& other) noexcept
{
	m_Other = other.m_Other;
	other.m_Other = nullptr;
}

ColliderComponent& ColliderComponent::operator=(const ColliderComponent& other)
{
	if (this == &other)
	{
		return *this;
	}
	m_Other = other.m_Other;
	return *this;
}

ColliderComponent& ColliderComponent::operator=(ColliderComponent&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	m_Other = other.m_Other;
	other.m_Other = nullptr;
	return *this;
}

std::shared_ptr<BaseComponent> ColliderComponent::Clone() const
{
	return nullptr;
}

bool ColliderComponent::IsOverlapping(std::shared_ptr<dae::GameObject>& other)
{
	if (!other)
	{
		SetOther(nullptr);
		return false;
	}
	return other->GetComponent<ColliderComponent>() != nullptr;
}


bool ColliderComponent::IsRaycasting(std::shared_ptr<dae::GameObject>& other)
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

bool ColliderComponent::GetIsStatic() const
{
	return m_IsStatic;
}

void ColliderComponent::SetIsStatic(bool isStatic)
{
	m_IsStatic = isStatic;
}

void ColliderComponent::SetOther(dae::GameObject* other)
{
	m_Other = other;
}
