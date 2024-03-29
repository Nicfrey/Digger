#include "GameObjectCommand.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

GameObjectCommand::GameObjectCommand(dae::GameObject* go): m_GameObject{go}
{
}

dae::GameObject* GameObjectCommand::GetGameObject() const
{
	return m_GameObject;
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
