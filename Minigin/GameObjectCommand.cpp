#include "GameObjectCommand.h"

#include "GameObject.h"

GameObjectCommand::GameObjectCommand(dae::GameObject* go): m_GameObject{go}
{
}

bool GameObjectCommand::IsDestroyed() const
{
	return m_GameObject->IsDestroyed();
}

dae::GameObject* GameObjectCommand::GetGameObject() const
{
	return m_GameObject;
}

