#include "DiggerCommands.h"
#include "GameObject.h"
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
