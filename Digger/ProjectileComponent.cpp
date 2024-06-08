#include "ProjectileComponent.h"

#include "DiggerUtils.h"
#include "GameObject.h"
#include "Observer.h"
#include "SoundSystemEngine.h"
#include "SpriteComponent.h"
#include "TimeEngine.h"

ProjectileComponent::ProjectileComponent(): BaseComponent{nullptr}, m_IsActive{}, m_RotationSpeed{15.f}, m_Velocity{}, m_Direction{}, m_ShotBy{nullptr}
{
}

ProjectileComponent::ProjectileComponent(const glm::vec2& direction): ProjectileComponent{}
{
	m_Direction = direction;
}

ProjectileComponent::ProjectileComponent(ProjectileComponent&& other) noexcept: BaseComponent{nullptr}
{
	m_IsActive = other.m_IsActive;
	m_RotationSpeed = other.m_RotationSpeed;
	m_Velocity = other.m_Velocity;
	m_Direction = other.m_Direction;
	m_ShotBy = std::move(other.m_ShotBy);
}

ProjectileComponent& ProjectileComponent::operator=(const ProjectileComponent& other)
{
	if (this == &other)
	{
		return *this;
	}

	m_IsActive = other.m_IsActive;
	m_RotationSpeed = other.m_RotationSpeed;
	m_Velocity = other.m_Velocity;
	m_Direction = other.m_Direction;
	m_ShotBy = other.m_ShotBy;

	return *this;
}

ProjectileComponent& ProjectileComponent::operator=(ProjectileComponent&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}

	m_IsActive = other.m_IsActive;
	m_RotationSpeed = other.m_RotationSpeed;
	m_Velocity = other.m_Velocity;
	m_Direction = other.m_Direction;
	m_ShotBy = std::move(other.m_ShotBy);

	return *this;
}

std::shared_ptr<BaseComponent> ProjectileComponent::Clone() const
{
		return std::make_shared<ProjectileComponent>(*this);
}

void ProjectileComponent::Update()
{
	if(m_IsActive)
	{
		m_Velocity = 100.f * TimeEngine::GetInstance().GetDeltaTime() * m_Direction;
		GetGameObject()->Translate(m_Velocity);
	}
}

void ProjectileComponent::Init()
{
	if (const auto spriteComp{ GetGameObject()->GetComponent<SpriteComponent>() })
	{
		spriteComp->SetTexture("SpritesFire.png");
	}
}


void ProjectileComponent::Activate(dae::GameObject* owner)
{
	m_IsActive = true;
	m_ShotBy = owner;
}
