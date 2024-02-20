#include <string>
#include "GameObject.h"

#include <iostream>

#include "ResourceManager.h"
#include "Renderer.h"
#include "TextureManager.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Init()
{
	for (const std::shared_ptr<GameObjectComponent> goc : m_Components)
	{
		goc->Init();
	}
}

void dae::GameObject::Update()
{
	for (const std::shared_ptr<GameObjectComponent> goc : m_Components)
	{
		goc->Update();
	}
}

void dae::GameObject::Render() const
{
	for (const std::shared_ptr<GameObjectComponent> goc : m_Components)
	{
		goc->Render();
	}
}

bool dae::GameObject::RemoveComponentAtIndex(size_t index)
{
	if(index < 0 || index >= m_Components.size())
	{
		std::cerr << index << " is out of range\n";
		return false;
	}
	m_Components.erase(m_Components.begin() + index);
	return true;
}

std::shared_ptr<dae::GameObject> dae::GameObject::GetThis()
{
	return shared_from_this();
}


void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

