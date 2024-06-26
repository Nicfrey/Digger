#include "GameObject.h"

#include <iostream>

#include "Collider2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "TextureComponent.h"

dae::GameObject::~GameObject() = default;

dae::GameObject::GameObject(const GameObject& other): GameObject{}
{
	for(auto& component : other.m_Components)
	{
		const auto newComp{ component->Clone() };
		m_Components.emplace_back(newComp);
	}
	m_Tag = other.m_Tag;
	m_LocalTransform = other.m_LocalTransform;
	m_WorldTransform = other.m_WorldTransform;
	if(other.m_ParentObject != nullptr)
	{
		m_ParentObject = new GameObject{ *other.m_ParentObject };
	}
	else
	{
		m_ParentObject = nullptr;
	}
	for(auto& child: other.m_ChildrenObject)
	{
		m_ChildrenObject.emplace_back(new GameObject{ *child });
	}
	m_PositionIsDirty = other.m_PositionIsDirty;
	m_RotationIsDirty = other.m_RotationIsDirty;
}

dae::GameObject::GameObject(GameObject&& other) noexcept
{
	m_Components = std::move(other.m_Components);
	m_Tag = std::move(other.m_Tag);
	m_LocalTransform = std::move(other.m_LocalTransform);
	m_WorldTransform = std::move(other.m_WorldTransform);
	m_ParentObject = std::move(other.m_ParentObject);
	m_ChildrenObject = std::move(other.m_ChildrenObject);
	m_PositionIsDirty = std::move(other.m_PositionIsDirty);
	m_RotationIsDirty = std::move(other.m_RotationIsDirty);
}

dae::GameObject& dae::GameObject::operator=(const GameObject& other)
{
	if (this == &other)
	{
		return *this;
	}
	for (auto& component : m_Components)
	{
		component.reset();
	}
	m_Components.clear();
	if(m_ParentObject != nullptr)
	{
		delete m_ParentObject;
		m_ParentObject = nullptr;
	}
	for (auto& child : m_ChildrenObject)
	{
		delete child;
		child = nullptr;
	}
	m_ChildrenObject.clear();

	for (auto& component : other.m_Components)
	{
		const auto newComp{ component->Clone() };
		m_Components.emplace_back(newComp);
	}
	m_Tag = other.m_Tag;
	m_LocalTransform = other.m_LocalTransform;
	m_WorldTransform = other.m_WorldTransform;
	if (other.m_ParentObject != nullptr)
	{
		m_ParentObject = new GameObject{ *other.m_ParentObject };
	}
	else
	{
		m_ParentObject = nullptr;
	}
	for (auto& child : other.m_ChildrenObject)
	{
		m_ChildrenObject.emplace_back(new GameObject{ *child });
	}
	m_PositionIsDirty = other.m_PositionIsDirty;
	m_RotationIsDirty = other.m_RotationIsDirty;
	return *this;
}

dae::GameObject& dae::GameObject::operator=(GameObject&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	for (auto& component : m_Components)
	{
		component.reset();
	}
	m_Components.clear();
	if (m_ParentObject != nullptr)
	{
		delete m_ParentObject;
		m_ParentObject = nullptr;
	}
	for (auto& child : m_ChildrenObject)
	{
		delete child;
		child = nullptr;
	}
	m_ChildrenObject.clear();

	m_Components = std::move(other.m_Components);
	m_Tag = std::move(other.m_Tag);
	m_LocalTransform = std::move(other.m_LocalTransform);
	m_WorldTransform = std::move(other.m_WorldTransform);
	m_ParentObject = std::move(other.m_ParentObject);
	m_ChildrenObject = std::move(other.m_ChildrenObject);
	m_PositionIsDirty = std::move(other.m_PositionIsDirty);
	m_RotationIsDirty = std::move(other.m_RotationIsDirty);
	return *this;
}

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

void dae::GameObject::RenderGUI()
{
	for (const std::shared_ptr<BaseComponent>& goc : m_Components)
	{
		goc->RenderGUI();
	}
}

void dae::GameObject::SetTag(const std::string& tag)
{
	m_Tag = tag;
}

std::string dae::GameObject::GetTag() const
{
	return m_Tag;
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

void dae::GameObject::Translate(const glm::vec3& pos)
{
	glm::vec3 localPos{ GetLocalPosition() };
	localPos += pos;
	SetLocalPosition(localPos);
}

void dae::GameObject::Translate(const glm::vec2& pos)
{
	Translate(glm::vec3{ pos.x,pos.y,0 });
}

void dae::GameObject::Translate(float x, float y, float z)
{
	Translate(glm::vec3{ x,y,z });
}

void dae::GameObject::Translate(float x, float y)
{
	Translate(glm::vec3{x,y,0});
}

glm::vec3 dae::GameObject::GetWorldRotation()
{
	if(m_RotationIsDirty)
	{
		UpdateWorldRotation();
	}
	return m_WorldTransform.GetRotation();
}

const glm::vec3& dae::GameObject::GetLocalRotation() const
{
	return m_LocalTransform.GetRotation();
}

void dae::GameObject::SetLocalRotation(const glm::vec3& rot)
{
	SetRotationIsDirty();
	m_LocalTransform.SetRotation(rot);
}

void dae::GameObject::SetLocalRotation(const glm::vec2& rot)
{
	SetLocalRotation(glm::vec3{ rot.x,rot.y,0 });
}

void dae::GameObject::SetLocalRotation(float x, float y, float z)
{
	SetLocalRotation(glm::vec3{ x,y,z });
}

void dae::GameObject::SetLocalRotation(float x, float y)
{
	SetLocalRotation(glm::vec3{ x,y,0 });
}

void dae::GameObject::OnCollisionEnter(std::shared_ptr<GameObject>& other)
{
	if (const auto & collider{ GetComponent<Collider2D>() })
	{
		for (const auto& goc : m_Components)
		{
			goc->OnCollisionEnter(other);
		}
	}
}

void dae::GameObject::OnCollisionExit(std::shared_ptr<GameObject>& other)
{
	if(const auto& collider{GetComponent<Collider2D>()})
	{
		for (const std::shared_ptr<BaseComponent>& goc : m_Components)
		{
			goc->OnCollisionExit(other);
		}
	}
}

void dae::GameObject::OnCollisionStay(std::shared_ptr<GameObject>& other)
{
	if (const auto & collider{ GetComponent<Collider2D>() })
	{
		for (const std::shared_ptr<BaseComponent>& goc : m_Components)
		{
			goc->OnCollisionStay(other);
		}
	}
}


std::shared_ptr<dae::GameObject> dae::GameObject::GetThis()
{
	return shared_from_this();
}

void dae::GameObject::SetPositionIsDirty()
{
	m_PositionIsDirty = true;
	for(const auto& child: m_ChildrenObject)
	{
		child->SetPositionIsDirty();
	}
}

void dae::GameObject::SetIsMoving()
{
	m_IsMoving = true;
	for (const auto& child : m_ChildrenObject)
	{
		child->SetIsMoving();
	}
}

bool dae::GameObject::GetIsMoving() const
{
	return m_IsMoving;
}

void dae::GameObject::SetIsNotMoving()
{
	m_IsMoving = false;
	for(const auto& child: m_ChildrenObject)
	{
		child->SetIsNotMoving();
	}
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

void dae::GameObject::SetRotationIsDirty()
{
	m_RotationIsDirty = true;
	for(const auto& child: m_ChildrenObject)
	{
		child->SetRotationIsDirty();
	}
}

void dae::GameObject::UpdateWorldRotation()
{
	if(m_RotationIsDirty)
	{
		if(m_ParentObject == nullptr)
		{
			m_WorldTransform.SetRotation(GetLocalRotation());
		}
		else
		{
			m_WorldTransform.SetRotation(m_ParentObject->GetWorldRotation() + GetLocalRotation());
		}
		m_RotationIsDirty = false;
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
		SetLocalRotation(GetWorldRotation());
	}
	else
	{
		if(keepWorldPosition)
		{
			SetLocalPosition(GetWorldPosition() - newParent->GetWorldPosition());
			SetLocalRotation(GetWorldRotation() - newParent->GetWorldRotation());
		}
		SetPositionIsDirty();
		SetRotationIsDirty();
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

std::vector<dae::GameObject*> dae::GameObject::GetChildren() const
{
	return m_ChildrenObject;
}

glm::vec3 dae::GameObject::GetForward() const
{
	return m_LocalTransform.GetForward();
}

void dae::GameObject::Destroy()
{
	m_IsDestroyed = true;
}

bool dae::GameObject::IsDestroyed() const
{
	return m_IsDestroyed;
}

void dae::GameObject::OnDestroy()
{
	for(const auto& goc : m_Components)
	{
		goc->OnDestroy();
	}
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
