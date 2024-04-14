#include "DiggerCommands.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "MathUtils.h"
#include "PlayerComponent.h"
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
	GetGameObject()->SetLocalRotation(0, 0, MathUtils::Rad2Deg(MathUtils::Atan2(m_Direction.y, m_Direction.x)));
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
