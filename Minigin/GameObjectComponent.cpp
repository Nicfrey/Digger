#include "GameObjectComponent.h"

GameObjectComponent::~GameObjectComponent() = default;

void GameObjectComponent::SetGameObject(const std::shared_ptr<dae::GameObject>& go)
{
	m_GameObject = go;
}
