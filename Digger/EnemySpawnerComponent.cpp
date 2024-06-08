#include "EnemySpawnerComponent.h"

#include <iostream>
#include <windows.h>
#include <Xinput.h>

#include "SceneManager.h"
#include "Scene.h"
#include "AnimatorComponent.h"
#include "BoxCollider2D.h"
#include "Controller.h"
#include "DiggerCommands.h"
#include "DiggerTransitionAnim.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "InputManager.h"
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
	EventManager::GetInstance().AddEvent("PlayerDied", this, &EnemySpawnerComponent::SetPlayerDead);
	EventManager::GetInstance().AddEvent("PlayerWon", this, &EnemySpawnerComponent::StopSpawning);
}

void EnemySpawnerComponent::Update()
{
	if(m_Player)
	{
		if(m_Player->IsDestroyed())
		{
			m_Player = nullptr;
		}
	}
}

void EnemySpawnerComponent::OnDestroy()
{
	EventManager::GetInstance().RemoveEvent("EnemyDied", this,&EnemySpawnerComponent::DecreaseEnemyCount);
	TimerManager::GetInstance().RemoveTimer(this, &EnemySpawnerComponent::SpawnNewEnemy, 5.f);
	EventManager::GetInstance().RemoveEvent("PlayerDied",this,&EnemySpawnerComponent::SetPlayerDead);
	EventManager::GetInstance().RemoveEvent("PlayerWon", this, &EnemySpawnerComponent::StopSpawning);
}

void EnemySpawnerComponent::CanSpawnPlayer()
{
	m_CanSpawnPlayer = true;
}

void EnemySpawnerComponent::CreateNewEnemy()
{
	const auto go = std::make_shared<dae::GameObject>();
	const auto sprite{ std::make_shared<SpriteComponent>("Enemy","SpritesEnemies.png",4,4) };
	const auto healthComponent = std::make_shared<HealthComponent>(1);
	const auto boxCollider = std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height);
	const auto enemyComponent{ std::make_shared<EnemyComponent>(EnemyComponent::EnemyType::Hobbins) };
	const auto animatorComponent{ std::make_shared<AnimatorComponent>() };
	Animation walkAnimationHobbins{ .name = "walkHobbins", .frames = {0,1,2}, .spriteComponent = sprite };
	Animation walkAnimationNobbins{ .name = "walkNobbins", .frames = {4,5,6}, . spriteComponent = sprite };
	Animation deadAnimationHobbins{ .name = "deadHobbins", .frames = {3}, .spriteComponent = sprite };
	Animation deadAnimationNobbins{ .name = "deadNobbins", .frames = {7}, .spriteComponent = sprite };
	TransitionEnemyIsDead* transitionDead{ new TransitionEnemyIsDead() };
	TransitionEnemyNobbinsTransformed* transitionNobbins{ new TransitionEnemyNobbinsTransformed() };
	TransitionEnemyHobbinsTransformed* transitionHobbins{ new TransitionEnemyHobbinsTransformed() };
	animatorComponent->AddParameter("IsDead", false);
	animatorComponent->AddParameter("EnemyType", enemyComponent->GetType());
	const auto levelObject = dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>();
	const auto navMeshAgentComponent{ std::make_shared<NavMeshAgentComponent>(levelObject->GetComponent<LevelComponent>()->GetGraph()) };
	const auto player = dae::SceneManager::GetInstance().GetGameObjectWithComponent<PlayerComponent>();
	go->SetLocalPosition(GetGameObject()->GetWorldPosition());
	go->AddComponent(sprite);
	go->AddComponent(healthComponent);
	go->AddComponent(boxCollider);
	go->AddComponent(enemyComponent);
	go->AddComponent(animatorComponent);
	go->AddComponent(navMeshAgentComponent);
	if(m_Player == nullptr && m_CanSpawnPlayer)
	{
		walkAnimationHobbins.frames = { 8,9,10 };
		deadAnimationHobbins.frames = { 11 };
		walkAnimationNobbins.frames = { 12,13,14 };
		deadAnimationNobbins.frames = { 15 };
		enemyComponent->IsPossessed();
		GamepadController* gamepadController{new GamepadController{1}}; // Always the second player
		auto moveUpCommand{ std::make_shared<MoveEnemyCommand>(go.get(),glm::vec2{0.f,-1.f})};
		auto moveDownCommand{ std::make_shared<MoveEnemyCommand>(go.get(),glm::vec2{0.f,1.f}) };
		auto moveRightCommand{ std::make_shared<MoveEnemyCommand>(go.get(),glm::vec2{1.f,0.f})};
		auto moveLeftCommand{ std::make_shared<MoveEnemyCommand>(go.get(),glm::vec2{-1.f,0.f}) };
		auto transformCommand{ std::make_shared<TransformEnemyCommand>(go.get()) };
		gamepadController->BindAction(moveUpCommand, XINPUT_GAMEPAD_DPAD_UP);
		gamepadController->BindAction(moveDownCommand, XINPUT_GAMEPAD_DPAD_DOWN);
		gamepadController->BindAction(moveRightCommand, XINPUT_GAMEPAD_DPAD_RIGHT);
		gamepadController->BindAction(moveLeftCommand, XINPUT_GAMEPAD_DPAD_LEFT);
		gamepadController->BindAction(transformCommand, XINPUT_GAMEPAD_B,KeyPressed);
		dae::InputManager::GetInstance().AddController(gamepadController);
		m_Player = go;
	}
	else
	{
		navMeshAgentComponent->SetPath(player->GetWorldPosition());
	}
	animatorComponent->AddTransition(walkAnimationNobbins, deadAnimationNobbins, transitionDead);
	animatorComponent->AddTransition(walkAnimationHobbins, deadAnimationHobbins, transitionDead);
	animatorComponent->AddTransition(walkAnimationHobbins, walkAnimationNobbins, transitionNobbins);
	animatorComponent->AddTransition(walkAnimationNobbins, walkAnimationHobbins, transitionHobbins);
	if (!animatorComponent->SetStartAnimation(walkAnimationHobbins))
	{
		std::cerr << "EnemySpawnerComponent::CreateNewEnemy() > Failed to set start animation\n";
	}
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

void EnemySpawnerComponent::SetPlayerDead()
{
	// Check if all players are dead
	const auto playersObject{ dae::SceneManager::GetInstance().GetGameObjectsWithComponent<PlayerComponent>() };
	size_t nbPlayers{};
	for(const auto& player: playersObject)
	{
		if(!player->HasComponent<HealthComponent>())
		{
			throw std::runtime_error("Player should always have a health component");
		}
		if(player->GetComponent<HealthComponent>()->IsDead())
		{
			++nbPlayers;
		}
	}
	if(nbPlayers == playersObject.size())
	{
		m_PlayerDead = true;
		TimerManager::GetInstance().RemoveTimer(this, &EnemySpawnerComponent::SpawnNewEnemy, 5.f);
		EventManager::GetInstance().RemoveEvent("PlayerWon", this, &EnemySpawnerComponent::SetPlayerDead);
	}
}

void EnemySpawnerComponent::StopSpawning()
{
	TimerManager::GetInstance().RemoveTimer(this, &EnemySpawnerComponent::SpawnNewEnemy, 5.f);
	EventManager::GetInstance().RemoveEvent("PlayerWon", this, &EnemySpawnerComponent::SetPlayerDead);
}
