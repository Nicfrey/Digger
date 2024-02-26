#include "BaseComponent.h"

void BaseComponent::SetGameObject(const std::shared_ptr<dae::GameObject>& go)
{
	m_GameObject = go.get();
}
