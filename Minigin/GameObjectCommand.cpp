#include "GameObjectCommand.h"

GameObjectCommand::GameObjectCommand(dae::GameObject* go): m_GameObject{go}
{
}

dae::GameObject* GameObjectCommand::GetGameObject() const
{
	return m_GameObject;
}
