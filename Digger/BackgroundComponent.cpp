#include "BackgroundComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "ProjectileComponent.h"
#include "SceneManager.h"

std::shared_ptr<BaseComponent> BackgroundComponent::Clone() const
{
	return std::make_shared<BackgroundComponent>(*this);
}

void BackgroundComponent::OnCollisionEnter(std::shared_ptr<dae::GameObject>& other)
{
	if(other->HasComponent<PlayerComponent>())
	{
		GetGameObject()->Destroy();
	}
	if(other->HasComponent<ProjectileComponent>())
	{
		const auto projectile{ other->GetComponent<ProjectileComponent>() };
		EventManager::GetInstance().NotifyEvent("ProjectileHit");
		const auto playerComp{ projectile->GetShotBy()->GetComponent<PlayerComponent>() };
		playerComp->ResetProjectile();
		other->Destroy();
	}
}
