#include "BaseComponent.h"

#include <cassert>

#include "GameObject.h"

BaseComponent::BaseComponent(const BaseComponent& other): BaseComponent(other.m_GameObject)
{

}

BaseComponent::BaseComponent(BaseComponent&& other) noexcept
{
	m_GameObject = std::move(other.m_GameObject);
}

BaseComponent& BaseComponent::operator=(const BaseComponent& other)
{
	if(this == &other)
	{
		return *this;
	}
	m_GameObject = other.m_GameObject;
	return *this;
}

BaseComponent& BaseComponent::operator=(BaseComponent&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}
	m_GameObject = std::move(other.m_GameObject);
	return *this;
}

void BaseComponent::SetGameObject(const std::shared_ptr<dae::GameObject>& go)
{
	assert(go);
	if(!go)
	{
		return;
	}
	if(m_GameObject)
	{
		m_GameObject->RemoveComponent(GetThis());
	}
	m_GameObject = go.get();
}

void BaseComponent::SetGameObject(dae::GameObject* go)
{
	assert(go);
	if(!go)
	{
		return;
	}
	if(m_GameObject)
	{
		m_GameObject->RemoveComponent(GetThis());
	}
	m_GameObject = go;
}

void BaseComponent::RemoveGameObject()
{
	m_GameObject = nullptr;
}

std::shared_ptr<BaseComponent> BaseComponent::GetThis()
{
	return shared_from_this();
}
