#include <string>
#include "GameObject.h"

#include <iostream>

#include "ResourceManager.h"
#include "Renderer.h"
#include "TextureManager.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Init()
{
	for (const std::shared_ptr<BaseComponent> goc : m_Components)
	{
		goc->Init();
	}
}

void dae::GameObject::Update()
{
	for (const std::shared_ptr<BaseComponent> goc : m_Components)
	{
		goc->Update();
	}
}

void dae::GameObject::FixedUpdate()
{
	for (const std::shared_ptr<BaseComponent> goc : m_Components)
	{
		goc->FixedUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const std::shared_ptr<BaseComponent> goc : m_Components)
	{
		goc->Render();
	}
}

bool dae::GameObject::RemoveComponentAtIndex(size_t index)
{
	if (index < 0 || index >= m_Components.size())
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

dae::Transform dae::GameObject::GetTransform() const
{
	return m_transform;
}

bool dae::GameObject::AddChild(const std::shared_ptr<GameObject>& child)
{
	assert(child);
	// TODO SLIDE 12
	if (!child)
	{
		return false;
	}

	m_ChildrenObject.emplace_back(child);
}

bool dae::GameObject::SetParent(const std::shared_ptr<GameObject>& newParent)
{
	assert(newParent);
	const auto newParentInChildren{
		std::find_if(m_ChildrenObject.begin(), m_ChildrenObject.end(),
					 [newParent](const std::shared_ptr<GameObject>& other)
					 {
						 return newParent == other;
					 })};
	if (!newParent && m_ParentObject == GetThis() && newParentInChildren != m_ChildrenObject.end())
	{
		return false;
	}

	if (m_ParentObject)
	{
		m_ParentObject = nullptr;
	}

	m_ParentObject = newParent;
	m_ParentObject->AddChild(GetThis());

	// TODO Update position

	return true;
}
