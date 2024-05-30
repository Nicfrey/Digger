#include "DiggerCommands.h"

#include <iostream>

#include "GameObject.h"
#include "Graph.h"
#include "HealthComponent.h"
#include "LevelComponent.h"
#include "MathUtils.h"
#include "NavMeshAgentComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TimeEngine.h"
#include "Utils.h"

MovePlayerCommand::MovePlayerCommand(dae::GameObject* go, glm::vec2 direction): GameObjectCommand{go}, m_CurrentNode{nullptr},
                                                                    m_TargetNode{nullptr}
{
	m_Direction = normalize(direction);
	if(go->HasComponent<NavMeshAgentComponent>())
	{
		go->GetComponent<NavMeshAgentComponent>()->SetSpeed(m_Speed);
	}
	else
	{
		std::cout << "MovePlayerCommand::MovePlayerCommand: No NavMeshAgentComponent found\n";
	}
}

void MovePlayerCommand::Execute()
{
	const auto levelObject{ dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>() };
	const auto levelComponent{ levelObject->GetComponent<LevelComponent>() };
	SetCurrentAndTargetNode(levelComponent);
	if(GetGameObject()->HasComponent<NavMeshAgentComponent>() && m_TargetNode != nullptr)
	{
		const auto navMeshAgent{ GetGameObject()->GetComponent<NavMeshAgentComponent>() };
		if(!levelComponent->IsNodeMoneyBag(m_TargetNode))
		{
			navMeshAgent->SetPath(m_TargetNode->GetPosition());
			GetGameObject()->SetLocalRotation(0, 0, MathUtils::Rad2Deg(MathUtils::Atan2(m_Direction.y, m_Direction.x)));
		}
	}
}

void MovePlayerCommand::SetCurrentAndTargetNode(const std::shared_ptr<LevelComponent>& levelComponent)
{
	m_CurrentNode = levelComponent->GetGraph()->GetClosestNode(GetGameObject()->GetWorldPosition());
	m_TargetNode = levelComponent->GetGraph()->GetNextNode(m_CurrentNode, m_Direction);
}

MoveEnemyCommand::MoveEnemyCommand(dae::GameObject* go, glm::vec2 direction) : MovePlayerCommand{ go, direction }
{
	m_Speed = 100.f;
	if (go->HasComponent<NavMeshAgentComponent>())
	{
		go->GetComponent<NavMeshAgentComponent>()->SetSpeed(m_Speed);
	}
	else
	{
		std::cout << "MoveEnemyCommand::MoveEnemyCommand: No NavMeshAgentComponent found\n";
	}
}

void MoveEnemyCommand::Execute()
{
	// TODO: Check if the enemy is transformed, if so, have the same behavior as the player (call MovePlayerCommand::Execute)
	const auto levelObject{ dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>() };
	const auto levelComponent{ levelObject->GetComponent<LevelComponent>() };
	SetCurrentAndTargetNode(levelComponent);
	if (GetGameObject()->HasComponent<NavMeshAgentComponent>() && m_TargetNode != nullptr)
	{
		const auto navMeshAgent{ GetGameObject()->GetComponent<NavMeshAgentComponent>() };
		if (m_TargetNode->CanBeVisited())
		{
			navMeshAgent->SetPath(m_TargetNode->GetPosition());
			GetGameObject()->SetLocalRotation(0, 0, MathUtils::Rad2Deg(MathUtils::Atan2(m_Direction.y, m_Direction.x)));
		}
	}
}

TransformEnemyCommand::TransformEnemyCommand(dae::GameObject* go): GameObjectCommand{go}
{
}

void TransformEnemyCommand::Execute()
{
	// TODO Check if the enemy component can transform
		// If so, transform the enemy (that calls a timer of how long he can transform)
}

ShootCommand::ShootCommand(dae::GameObject* go): GameObjectCommand{ go }
{
}

void ShootCommand::Execute()
{
	if(const auto playerComp{GetGameObject()->GetComponent<PlayerComponent>()})
	{
		playerComp->ShootProjectile();
	}
}
