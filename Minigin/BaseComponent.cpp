#include "BaseComponent.h"

#include <cassert>

#include "GameObject.h"

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

void BaseComponent::RemoveGameObject()
{
	m_GameObject = nullptr;
}

std::shared_ptr<BaseComponent> BaseComponent::GetThis()
{
	return shared_from_this();
}
