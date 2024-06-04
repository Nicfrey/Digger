#include "DiggerCommands.h"

#include <iostream>

#include "ButtonComponent.h"
#include "GameObject.h"
#include "Graph.h"
#include "HealthComponent.h"
#include "KeyboardComponent.h"
#include "LevelComponent.h"
#include "MathUtils.h"
#include "NavMeshAgentComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TimeEngine.h"
#include "Utils.h"
#include "Widget.h"
#include "WidgetManager.h"

MovePlayerCommand::MovePlayerCommand(dae::GameObject* go, glm::vec2 direction): GameObjectCommand{go},
	m_CurrentNode{nullptr},
	m_TargetNode{nullptr}
{
	m_Direction = normalize(direction);
	if (go->HasComponent<NavMeshAgentComponent>())
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
	const auto levelObject{dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>()};
	const auto levelComponent{levelObject->GetComponent<LevelComponent>()};
	SetCurrentAndTargetNode(levelComponent);
	if (GetGameObject()->HasComponent<NavMeshAgentComponent>() && m_TargetNode != nullptr)
	{
		const auto navMeshAgent{GetGameObject()->GetComponent<NavMeshAgentComponent>()};
		if (!levelComponent->IsNodeMoneyBag(m_TargetNode))
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

MoveEnemyCommand::MoveEnemyCommand(dae::GameObject* go, glm::vec2 direction) : MovePlayerCommand{go, direction}
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
	const auto levelObject{dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>()};
	const auto levelComponent{levelObject->GetComponent<LevelComponent>()};
	SetCurrentAndTargetNode(levelComponent);
	if (GetGameObject()->HasComponent<NavMeshAgentComponent>() && m_TargetNode != nullptr)
	{
		const auto navMeshAgent{GetGameObject()->GetComponent<NavMeshAgentComponent>()};
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

MoveButtonCommand::MoveButtonCommand(bool isUp): m_Up{isUp}
{
}

void MoveButtonCommand::Execute()
{
	const auto currentWidget = WidgetManager::GetInstance().GetActiveWidget();
	if(currentWidget == nullptr)
	{
		return;
	}
	const auto buttons = currentWidget->GetAllElement<ButtonComponent>();
	if (buttons.empty())
	{
		return;
	}
	if (m_Up)
	{
		for (size_t i{}; i < buttons.size(); ++i)
		{
			if (buttons[i]->IsSelected())
			{
				if (i + 1 >= buttons.size())
				{
					buttons[i]->DeselectButton();
					buttons[0]->SelectButton();
					break;
				}
				buttons[i]->SelectButton();
				break;
			}
		}
	}
	else
	{
		for (size_t i{}; i < buttons.size(); ++i)
		{
			if (buttons[i]->IsSelected())
			{
				if (i == 0)
				{
					buttons[i]->DeselectButton();
					buttons[buttons.size() - 1]->SelectButton();
					break;
				}
				buttons[i]->DeselectButton();
				buttons[i - 1]->SelectButton();
				break;
			}
		}
	}
}

void SelectButtonCommand::Execute()
{
	HandleButtonWidget();
	HandleButtonKeyboard();
}

void SelectButtonCommand::HandleButtonWidget()
{
	const auto currentWidget = WidgetManager::GetInstance().GetActiveWidget();
	if(currentWidget == nullptr)
	{
		return;
	}
	const auto buttons = currentWidget->GetAllElement<ButtonComponent>();
	if (buttons.empty())
	{
		return;
	}
	for (const auto& button : buttons)
	{
		if (button->IsSelected())
		{
			button->OnPressed();
			break;
		}
	}
}

void SelectButtonCommand::HandleButtonKeyboard()
{
	const auto currentWidget = WidgetManager::GetInstance().GetActiveWidget();
	if(currentWidget == nullptr)
	{
		return;
	}
	const auto keyboards = currentWidget->GetAllElement<KeyboardComponent>();
	if (keyboards.empty())
	{
				return;
	}
	for (const auto& keyboard : keyboards)
	{
		keyboard->OnPressed();
	}
}

MoveKeyboardCommand::MoveKeyboardCommand(const glm::ivec2& direction): m_Direction{direction}
{
}

void MoveKeyboardCommand::Execute()
{
	const auto currentWidget = WidgetManager::GetInstance().GetActiveWidget();
	if(currentWidget == nullptr)
	{
		return;
	}
	const auto keyboards = currentWidget->GetAllElement<KeyboardComponent>();
	for(auto& keyboard: keyboards)
	{
		keyboard->MoveSelection(m_Direction);
	}
}

ShootCommand::ShootCommand(dae::GameObject* go): GameObjectCommand{go}
{
}

void ShootCommand::Execute()
{
	if (const auto playerComp{GetGameObject()->GetComponent<PlayerComponent>()})
	{
		playerComp->ShootProjectile();
	}
}
