#include <string>
#include "GameObject.h"

#include <iostream>

#include "ResourceManager.h"
#include "Renderer.h"
#include "TextureManager.h"

dae::GameObject::~GameObject() = default;

bool dae::GameObject::IsEqualToParent(const std::shared_ptr<GameObject>& child)
{
	std::shared_ptr parent{ child->GetParent() };
	while(parent != nullptr)
	{
		if(parent == child)
		{
			return true;
		}
		parent = parent->GetParent();
	}
	return false;
}

void dae::GameObject::Init()
{
	for (const std::shared_ptr<BaseComponent>& goc : m_Components)
	{
		goc->Init();
	}
}

void dae::GameObject::Update()
{
	for (const std::shared_ptr<BaseComponent>& goc : m_Components)
	{
		goc->Update();
	}
}

void dae::GameObject::FixedUpdate()
{
	for (const std::shared_ptr<BaseComponent>& goc : m_Components)
	{
		goc->FixedUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const std::shared_ptr<BaseComponent>& goc : m_Components)
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
	// Check if child is equal to parent or if it's nullptr
	if(IsEqualToParent(child) && !child)
	{
		return false;
	}

	// Remove the parent from new child
	child->GetParent()->RemoveChild(child);

	// Add the child to the list of children
	m_ChildrenObject.emplace_back(child);

	// TODO update transform

	return true;
}

bool dae::GameObject::SetParent(const std::shared_ptr<GameObject>& newParent)
{
	assert(newParent);
	// Check if newParent is equal to current parent or if newParent is present in the children list
	const auto newParentInChildren{
		std::ranges::find_if(m_ChildrenObject,
		                     [newParent](const std::shared_ptr<GameObject>& other)
		                     {
			                     return newParent == other;
		                     })};
	if (m_ParentObject == shared_from_this() || newParentInChildren != m_ChildrenObject.end())
	{
		return false;
	}

	// Remove itself from previous parent if exists
	if(m_ParentObject)
	{
		m_ParentObject->RemoveChild(shared_from_this());
	}

	m_ParentObject = newParent;

	// If the parent is not nullptr
	if(m_ParentObject)
	{
		m_ParentObject->AddChild(shared_from_this());
	}

	// TODO Update position

	return true;
}

std::shared_ptr<dae::GameObject> dae::GameObject::GetParent() const
{
	return m_ParentObject;
}

bool dae::GameObject::RemoveChild(const std::shared_ptr<GameObject>& child)
{
	assert(child);
	if(!child)
	{
		std::cerr << "RemoveChild: child is nullptr\n";
		return false;
	}

	const auto it = std::ranges::find_if(m_ChildrenObject, [child](const auto& other)
	{
		return child == other;
	});

	if(it != m_ChildrenObject.end())
	{
		m_ChildrenObject.erase(it);
		child->SetParent(nullptr);
		return true;
	}
	return false;
}
