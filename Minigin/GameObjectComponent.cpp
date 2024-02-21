#include "GameObjectComponent.h"

void GameObjectComponent::SetGameObject(const std::shared_ptr<dae::GameObject>& go)
{
	m_GameObject = go;
}
