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
	return std::make_shared<ColliderComponent>(*this);
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

dae::GameObject* ColliderComponent::GetOther() const
{
	return m_Other;
}

void ColliderComponent::SetOther(dae::GameObject* other)
{
	m_Other = other;
}
