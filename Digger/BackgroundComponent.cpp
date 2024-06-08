#include "BackgroundComponent.h"

#include "DiggerUtils.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "Graph.h"
 #include "LevelComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "ProjectileComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SoundSystemEngine.h"

std::shared_ptr<BaseComponent> BackgroundComponent::Clone() const
{
	return std::make_shared<BackgroundComponent>(*this);
}

void BackgroundComponent::OnCollisionEnter(std::shared_ptr<dae::GameObject>& other)
{
	HandlePlayerCollision(other);
	HandleProjectileCollision(other);
	HandleEnemyCollision(other);
}

void BackgroundComponent::HandlePlayerCollision(std::shared_ptr<dae::GameObject>& other) const
{
	if (other->HasComponent<PlayerComponent>())
	{
		GetGameObject()->Destroy();
	}
}

void BackgroundComponent::HandleProjectileCollision(std::shared_ptr<dae::GameObject>& other)
{
	if (other->HasComponent<ProjectileComponent>())
	{
		const auto projectile{ other->GetComponent<ProjectileComponent>() };
		const auto playerComp{ projectile->GetShotBy()->GetComponent<PlayerComponent>() };
		other->Destroy();
		ServiceSoundLocator::GetSoundSystem().Play(TO_SOUND_ID(DiggerUtils::SoundDiggerID::PROJECTILE_HIT_WALL),50);
		EventManager::GetInstance().NotifyEvent("ProjectileHit");
	}
}

void BackgroundComponent::HandleEnemyCollision(std::shared_ptr<dae::GameObject>& other) const
{
	if (other->HasComponent<EnemyComponent>())
	{
		if (other->GetComponent<EnemyComponent>()->GetType() == EnemyComponent::EnemyType::Nobbins)
		{
			GetGameObject()->Destroy();
		}
	}
}
