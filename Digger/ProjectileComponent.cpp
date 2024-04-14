#include "ProjectileComponent.h"

#include "GameObject.h"
#include "SpriteComponent.h"
#include "TimeEngine.h"

ProjectileComponent::ProjectileComponent(): BaseComponent{nullptr}, m_IsActive{}, m_RotationSpeed{15.f}, m_Velocity{}, m_Direction{}, m_ShotBy{nullptr}
{
}

ProjectileComponent::ProjectileComponent(const glm::vec2& direction): ProjectileComponent{}
{
	m_Direction = direction;
}

void ProjectileComponent::Update()
{
	if(m_IsActive)
	{
		m_Velocity = 20.f * TimeEngine::GetInstance().GetDeltaTime() * m_Direction;
		GetGameObject()->Translate(m_Velocity);
	}
}

void ProjectileComponent::Init()
{
	if (const auto spriteComp{ GetGameObject()->GetComponent<SpriteComponent>() })
	{
		spriteComp->SetTexture("Projectile.png");
	}
}

void ProjectileComponent::Activate(dae::GameObject* owner)
{
	m_IsActive = true;
	m_ShotBy = owner;
}	
