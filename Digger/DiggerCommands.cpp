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
		if(!levelComponent->IsNodeMoneyBag(m_TargetNode))
		{
			navMeshAgent->SetPath(m_TargetNode->GetPosition());
			GetGameObject()->SetLocalRotation(0, 0, MathUtils::Rad2Deg(MathUtils::Atan2(m_Direction.y, m_Direction.x)));
		}
	}
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
