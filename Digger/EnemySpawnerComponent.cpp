#include "EnemySpawnerComponent.h"

#include <iostream>

#include "SceneManager.h"
#include "Scene.h"
#include "AnimatorComponent.h"
#include "BoxCollider2D.h"
#include "DiggerTransitionAnim.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "LevelComponent.h"
#include "NavMeshAgentComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "SpriteComponent.h"
#include "TimeEngine.h"
#include "Utils.h"

std::shared_ptr<BaseComponent> EnemySpawnerComponent::Clone() const
{
	return std::make_shared<EnemySpawnerComponent>(*this);
}

void EnemySpawnerComponent::Init()
{
	EventManager::GetInstance().AddEvent("EnemyDied", this,&EnemySpawnerComponent::DecreaseEnemyCount);
	TimerManager::GetInstance().AddTimer(this, &EnemySpawnerComponent::SpawnNewEnemy, 5.f,true);
}

void EnemySpawnerComponent::Update()
{
}

void EnemySpawnerComponent::OnDestroy()
{
	EventManager::GetInstance().RemoveEvent("EnemyDied", this,&EnemySpawnerComponent::DecreaseEnemyCount);
	TimerManager::GetInstance().RemoveTimer(this, &EnemySpawnerComponent::SpawnNewEnemy, 5.f);
}

void EnemySpawnerComponent::CreateNewEnemy()
{
	const auto go = std::make_shared<dae::GameObject>();
	const auto sprite{ std::make_shared<SpriteComponent>("Enemy","SpritesEnemies.png",4,2) };
	const auto healthComponent = std::make_shared<HealthComponent>(1);
	const auto boxCollider = std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height);
	const auto enemyComponent{ std::make_shared<EnemyComponent>(EnemyComponent::EnemyType::Nobbins) };
	const auto animatorComponent{ std::make_shared<AnimatorComponent>() };
	Animation walkAnimation{ .name = "walk", .frames = {0,1,2}, .spriteComponent = sprite };
	Animation deadAnimation{ .name = "dead", .frames = {3}, .spriteComponent = sprite };
	if(enemyComponent->GetType() == EnemyComponent::EnemyType::Hobbins)
	{
		walkAnimation.frames = {5,6,7};
		deadAnimation.frames = {8};
	}
	TransitionEnemyIsDead* transition = new TransitionEnemyIsDead();
	animatorComponent->AddTransition(walkAnimation,deadAnimation,transition);
	animatorComponent->AddParameter("EnemyIsDead", false);
	if(!animatorComponent->SetStartAnimation(walkAnimation))
	{
		std::cerr << "EnemySpawnerComponent::CreateNewEnemy() > Failed to set start animation\n";
	}
	const auto levelObject = dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>();
	const auto navMeshAgentComponent{ std::make_shared<NavMeshAgentComponent>(levelObject->GetComponent<LevelComponent>()->GetGraph()) };
	const auto player = dae::SceneManager::GetInstance().GetGameObjectWithComponent<PlayerComponent>();
	go->AddComponent(sprite);
	go->AddComponent(healthComponent);
	go->AddComponent(boxCollider);
	go->AddComponent(enemyComponent);
	go->AddComponent(animatorComponent);
	go->AddComponent(navMeshAgentComponent);
	go->SetLocalPosition(GetGameObject()->GetWorldPosition());
	navMeshAgentComponent->SetPath(player->GetWorldPosition());
	go->SetTag("Enemy");
	dae::SceneManager::GetInstance().Instantiate(go);
}

void EnemySpawnerComponent::SpawnNewEnemy()
{
	if(m_EnemyToSpawn <= 0)
	{
		GetGameObject()->Destroy();
		return;
	}
	CreateNewEnemy();
	--m_EnemyToSpawn;
	++m_EnemyCount;
}


void EnemySpawnerComponent::DecreaseEnemyCount()
{
	--m_EnemyCount;
}