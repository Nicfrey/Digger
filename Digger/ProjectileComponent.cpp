#include "ProjectileComponent.h"

#include "GameObject.h"
#include "SpriteComponent.h"

void ProjectileComponent::Update()
{

}

void ProjectileComponent::Init()
{
	if (const auto spriteComp{ GetGameObject()->GetComponent<SpriteComponent>() })
	{
		spriteComp->SetTexture("Projectile.png");
	}
}

void ProjectileComponent::OnCollisionEnter(dae::GameObject* other)
{
	if(other)
	{
		
	}
	// TODO Check if the component is enemy component
	// Kill the enemy
}
