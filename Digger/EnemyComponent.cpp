#include "EnemyComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "ProjectileComponent.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "SpriteComponent.h"

int EnemyComponent::m_ScoreKilled{ 250 };

EnemyComponent::EnemyComponent(): BaseComponent{nullptr}
{
	EventManager::GetInstance().AddEvent("EnemyDied", std::bind(&EnemyComponent::OnEnemyDead,this));
}

EnemyComponent::EnemyComponent(EnemyType type): EnemyComponent()
{
	m_Type = type;
}

void EnemyComponent::Init()
{
	if(GetGameObject()->HasComponent<SpriteComponent>())
	{
		const auto sprite{ GetGameObject()->GetComponent<SpriteComponent>() };
		switch (m_Type)
		{
		case EnemyType::Nobbins:
			sprite->SetCurrentRow(0);
			break;
		case EnemyType::Hobbins:
			sprite->SetCurrentRow(1);
			break;
		}
	}
}

std::shared_ptr<BaseComponent> EnemyComponent::Clone() const
{
	return std::make_shared<EnemyComponent>(*this);
}

void EnemyComponent::OnCollisionEnter(std::shared_ptr<dae::GameObject>& other)
{
	if(other->IsDestroyed())
	{
		return;
	}

	if(other->HasComponent<ProjectileComponent>())
	{
		const auto projectile = other->GetComponent<ProjectileComponent>();
		if(projectile->GetShotBy()->HasComponent<PlayerComponent>() && projectile->GetShotBy()->HasComponent<ScoreComponent>())
		{
			const auto scoreComp{ projectile->GetShotBy()->GetComponent<ScoreComponent>() };
			const auto playerComp{ projectile->GetShotBy()->GetComponent<PlayerComponent>() };
			playerComp->ResetProjectile();
			scoreComp->AddScore(m_ScoreKilled);
			other->Destroy();
			if(GetGameObject()->HasComponent<HealthComponent>())
			{
				const auto health{ GetGameObject()->GetComponent<HealthComponent>() };
				health->LoseOneLife();
			}
		}
	}
}

void EnemyComponent::OnEnemyDead()
{
	if(GetGameObject()->HasComponent<HealthComponent>())
	{
		const auto health{ GetGameObject()->GetComponent<HealthComponent>() };
		if(health->IsDead())
		{
			GetGameObject()->Destroy();
		}
	}
}

void EnemyComponent::OnDestroy()
{
	EventManager::GetInstance().RemoveEvent("EnemyDied", [this]() { OnEnemyDead(); });
}
