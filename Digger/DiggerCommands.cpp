#include "DiggerCommands.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "TimeEngine.h"

MoveCommand::MoveCommand(dae::GameObject* go, glm::vec2 direction): GameObjectCommand{go}
{
	m_Direction = normalize(direction);
}

void MoveCommand::Execute()
{
	const glm::vec2 velocity{ m_Direction * TimeEngine::GetInstance().GetDeltaTime() * m_Speed };
	GetGameObject()->Translate(velocity);
}

KillPlayerCommand::KillPlayerCommand(dae::GameObject* go) : GameObjectCommand{ go }
{
}

void KillPlayerCommand::Execute()
{
	if (const auto healthComponent{ GetGameObject()->GetComponent<HealthComponent>() })
	{
		healthComponent->LoseOneLife();
	}
}

AddScorePlayerCommand::AddScorePlayerCommand(dae::GameObject* go) : GameObjectCommand{ go }
{
}

void AddScorePlayerCommand::Execute()
{
	if (const auto scoreComponent{ GetGameObject()->GetComponent<ScoreComponent>() })
	{
		scoreComponent->AddScore(100);
	}
}