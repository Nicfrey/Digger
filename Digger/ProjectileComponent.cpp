#include "ProjectileComponent.h"

#include "GameObject.h"
#include "SpriteComponent.h"
#include "TimeEngine.h"

ProjectileComponent::ProjectileComponent(): BaseComponent{nullptr}, m_IsActive{}, m_RotationSpeed{15.f}, m_Velocity{}
{
}

void ProjectileComponent::Update()
{
	if(m_IsActive)
	{
		m_Velocity.x = 20.f * TimeEngine::GetInstance().GetDeltaTime();
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

void ProjectileComponent::Activate()
{
	m_IsActive = true;
}	
