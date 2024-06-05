#include "EnemyComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "MathUtils.h"
#include "NavMeshAgentComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "ProjectileComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "SpriteComponent.h"

int EnemyComponent::m_ScoreKilled{ 250 };

EnemyComponent::EnemyComponent(): BaseComponent{nullptr}
{
}

EnemyComponent::EnemyComponent(EnemyType type): EnemyComponent()
{
	m_Type = type;
}

void EnemyComponent::StopMoving()
{
	// Check if all players are dead
	size_t playersDead{};
	const auto players{ dae::SceneManager::GetInstance().GetGameObjectsWithComponent<PlayerComponent>() };
	for(const auto& player :players)
	{
		if(player->GetComponent<HealthComponent>()->IsDead())
		{
			++playersDead;
		}
	}
	if(playersDead == players.size())
	{
		m_StopMoving = true;
		m_pNavMeshAgent->StopMoving();
	}
}

void EnemyComponent::Init()
{
	EventManager::GetInstance().AddEvent("EnemyDied", this,&EnemyComponent::HandleDeadEnemy);
	EventManager::GetInstance().AddEvent("PlayerDied", this, &EnemyComponent::StopMoving);
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
	m_pNavMeshAgent = GetGameObject()->GetComponent<NavMeshAgentComponent>().get();
}

void EnemyComponent::Update()
{
	if(m_pNavMeshAgent->HasReachedDestination() && !m_StopMoving)
	{
		const auto players{ dae::SceneManager::GetInstance().GetGameObjectsWithComponent<PlayerComponent>() };
		const int index{MathUtils::Rand(0,players.size() - 1)};
		m_pNavMeshAgent->SetPath(players[index]->GetWorldPosition());
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
			scoreComp->AddScore(m_ScoreKilled);
			other->Destroy();
			if(GetGameObject()->HasComponent<HealthComponent>())
			{
				const auto health{ GetGameObject()->GetComponent<HealthComponent>() };
				health->LoseOneLife();
			}
			EventManager::GetInstance().NotifyEvent("ProjectileHit");
		}
	}
}

void EnemyComponent::HandleDeadEnemy() 
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
	EventManager::GetInstance().RemoveEvent("EnemyDied", this,&EnemyComponent::HandleDeadEnemy);
	EventManager::GetInstance().RemoveEvent("PlayerDied", this, &EnemyComponent::StopMoving);
}

EnemyComponent::EnemyType EnemyComponent::GetType() const
{
	return m_Type;
}
