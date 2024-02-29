#include <string>
#include "GameObject.h"

#include <iostream>

#include "ResourceManager.h"
#include "Renderer.h"
#include "TextureComponent.h"

dae::GameObject::~GameObject() = default;

bool dae::GameObject::IsEqualToParent(const std::shared_ptr<GameObject>& child)
{
	GameObject* parent{ child->GetParent() };
	while(parent != nullptr)
	{
		if(parent == child.get())
		{
			return true;
		}
		parent = parent->GetParent();
	}
	return false;
}

bool dae::GameObject::IsChild(const std::shared_ptr<GameObject>& parent)
{
	const auto it{
		std::ranges::find_if(m_ChildrenObject,
							 [parent](const GameObject* other)
							 {
								 return parent.get() == other;
							 }) };
	return it != m_ChildrenObject.end();
}

dae::GameObject::GameObject(const glm::vec3& pos): GameObject{}
{
	SetLocalPosition(pos);
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


glm::vec3 dae::GameObject::GetWorldPosition()
{
	if(m_PositionIsDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldTransform.GetPosition();
}

const glm::vec3& dae::GameObject::GetLocalPosition() const
{
	return m_LocalTransform.GetPosition();
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	SetPositionIsDirty();
	m_LocalTransform.SetPosition(pos);
}

void dae::GameObject::SetLocalPosition(const glm::vec2& pos)
{
	SetLocalPosition(glm::vec3{ pos.x,pos.y,0.f });
}

void dae::GameObject::SetLocalPosition(float x, float y, float z)
{
	SetLocalPosition(glm::vec3{ x,y,z });
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	SetLocalPosition(glm::vec3{ x,y,0.f });
}

std::shared_ptr<dae::GameObject> dae::GameObject::GetThis()
{
	return shared_from_this();
}

void dae::GameObject::SetPositionIsDirty()
{
	m_PositionIsDirty = true;
}

void dae::GameObject::UpdateWorldPosition()
{
	if(m_PositionIsDirty)
	{
		if(m_ParentObject == nullptr)
		{
			m_WorldTransform.SetPosition(GetLocalPosition());
		}
		else
		{
			m_WorldTransform.SetPosition(m_ParentObject->GetWorldPosition() + GetLocalPosition());
		}
		m_PositionIsDirty = false;
	}
}

bool dae::GameObject::AddChild(const std::shared_ptr<GameObject>& child)
{
	assert(child);
	// Check if child is equal to parent or if it's nullptr
	if(IsEqualToParent(child) || !child)
	{
		return false;
	}

	// Remove the parent from new child
	child->GetParent()->RemoveChild(child);

	// Add the child to the list of children
	m_ChildrenObject.emplace_back(child.get());

	// TODO update transform

	return true;
}

bool dae::GameObject::SetParent(const std::shared_ptr<GameObject>& newParent, bool keepWorldPosition)
{
	assert(newParent);
	// Check if newParent is equal to current parent or if newParent is present in the children list
	
	if (m_ParentObject == newParent.get() || IsChild(newParent) || newParent == GetThis())
	{
		return false;
	}

	// Remove itself from previous parent if exists
	if(newParent == nullptr)
	{
		SetLocalPosition(GetWorldPosition());
	}
	else
	{
		if(keepWorldPosition)
		{
			SetLocalPosition(GetWorldPosition() - newParent->GetWorldPosition());
		}
		SetPositionIsDirty();
	}
	if(m_ParentObject)
	{
		m_ParentObject->RemoveChild(shared_from_this());
	}

	m_ParentObject = newParent.get();

	// If the parent is not nullptr
	if(m_ParentObject)
	{
		m_ParentObject->AddChild(shared_from_this());
	}

	return true;
}

dae::GameObject* dae::GameObject::GetParent() const
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
		return child.get() == other;
	});

	if(it != m_ChildrenObject.end())
	{
		m_ChildrenObject.erase(it);
		child->SetParent(nullptr);
		return true;
	}
	return false;
}
