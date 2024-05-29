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

MoveCommand::MoveCommand(dae::GameObject* go, glm::vec2 direction): GameObjectCommand{go}, m_CurrentNode{nullptr},
                                                                    m_TargetNode{nullptr}
{
	m_Direction = normalize(direction);
	if(go->HasComponent<NavMeshAgentComponent>())
	{
		go->GetComponent<NavMeshAgentComponent>()->SetSpeed(m_Speed);
	}
	else
	{
		std::cout << "MoveCommand::MoveCommand: No NavMeshAgentComponent found\n";
	}
}

void MoveCommand::Execute()
{
	const auto levelObject{ dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>() };
	const auto levelComponent{ levelObject->GetComponent<LevelComponent>() };
	m_CurrentNode = levelComponent->GetGraph()->GetClosestNode(GetGameObject()->GetWorldPosition());
	m_TargetNode = levelComponent->GetGraph()->GetNextNode(m_CurrentNode, m_Direction);
	if(GetGameObject()->HasComponent<NavMeshAgentComponent>() && m_TargetNode != nullptr)
	{
		const auto navMeshAgent{ GetGameObject()->GetComponent<NavMeshAgentComponent>() };
		navMeshAgent->SetPath(m_TargetNode->GetPosition());
		GetGameObject()->SetLocalRotation(0, 0, MathUtils::Rad2Deg(MathUtils::Atan2(m_Direction.y, m_Direction.x)));
	}
	/*
	// get the current node if not
	const auto levelObject{ dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>() };
	const auto levelComponent{ levelObject->GetComponent<LevelComponent>() };
	m_CurrentNode = levelComponent->GetGraph()->GetClosestNode(GetGameObject()->GetWorldPosition());
	m_TargetNode = levelComponent->GetGraph()->GetNextNode(m_CurrentNode, m_Direction);
	if(m_TargetNode != nullptr || !IsPointInCircle(GetGameObject()->GetWorldPosition(),Circlef{GetGameObject()->GetWorldPosition(),m_AcceptanceRadius}))
	{
		std::cout << "Position current node: (" << m_CurrentNode->GetPosition().x << ", " << m_CurrentNode->GetPosition().y << ")\n";
		std::cout << "Position target node: (" << m_TargetNode->GetPosition().x << ", " << m_TargetNode->GetPosition().y << ")\n";
		const auto nodes{ levelComponent->GetGraph()->GetShortestPath(m_CurrentNode,m_TargetNode,true) };
		std::cout << "nodes: " << nodes.size() << "\n";
		const auto nextNode{ levelComponent->GetGraph()->GetShortestPath(m_CurrentNode,m_TargetNode,true)[1]};
		const glm::vec2 directionToNextNode{ normalize(nextNode->GetPosition() - m_CurrentNode->GetPosition()) };
		const glm::vec2 velocity{ directionToNextNode * TimeEngine::GetInstance().GetDeltaTime() * m_Speed };
		GetGameObject()->Translate(velocity);
		GetGameObject()->SetLocalRotation(0, 0, MathUtils::Rad2Deg(MathUtils::Atan2(directionToNextNode.y, directionToNextNode.x)));
		EventManager::GetInstance().NotifyEvent("PlayerMoving");
	}
	*/
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
