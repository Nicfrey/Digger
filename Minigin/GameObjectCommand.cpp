#include "GameObjectCommand.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "MovementComponent.h"
#include "ScoreComponent.h"

GameObjectCommand::GameObjectCommand(dae::GameObject* go): m_GameObject{go}
{
}

dae::GameObject* GameObjectCommand::GetGameObject() const
{
	return m_GameObject;
}

MoveForwardCommand::MoveForwardCommand(dae::GameObject* go): GameObjectCommand{go}
{
}

void MoveForwardCommand::Execute()
{
	if(const std::shared_ptr movComponent{ GetGameObject()->GetComponent<MovementComponent>() })
	{
		movComponent->Move(Direction::Up);
	}
}

MoveBackwardCommand::MoveBackwardCommand(dae::GameObject* go) : GameObjectCommand{ go }
{
}

void MoveBackwardCommand::Execute()
{
	if (const std::shared_ptr movComponent{ GetGameObject()->GetComponent<MovementComponent>() })
	{
		movComponent->Move(Direction::Down);
	}
}

MoveRightCommand::MoveRightCommand(dae::GameObject* go) : GameObjectCommand{ go }
{
}

void MoveRightCommand::Execute()
{
	if (const std::shared_ptr movComponent{ GetGameObject()->GetComponent<MovementComponent>() })
	{
		movComponent->Move(Direction::Right);
	}
}

MoveLeftCommand::MoveLeftCommand(dae::GameObject* go): GameObjectCommand{go}
{
}

void MoveLeftCommand::Execute()
{
	if (const std::shared_ptr movComponent{ GetGameObject()->GetComponent<MovementComponent>() })
	{
		movComponent->Move(Direction::Left);
	}
}

KillPlayerCommand::KillPlayerCommand(dae::GameObject* go): GameObjectCommand{go}
{
}

void KillPlayerCommand::Execute()
{
	if(const auto healthComponent{GetGameObject()->GetComponent<HealthComponent>()})
	{
		healthComponent->LoseOneLife();
	}
}

AddScorePlayerCommand::AddScorePlayerCommand(dae::GameObject* go): GameObjectCommand{ go }
{
}

void AddScorePlayerCommand::Execute()
{
	if (const auto scoreComponent{ GetGameObject()->GetComponent<ScoreComponent>() })
	{
		scoreComponent->AddScore(100);
	}
}
