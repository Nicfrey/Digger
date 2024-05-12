#include "MainMenuComponent.h"

#include "GameObject.h"
#include "LevelComponent.h"

std::shared_ptr<BaseComponent> MainMenuComponent::Clone() const
{
	return std::make_shared<MainMenuComponent>(*this);
}

void MainMenuComponent::Init()
{
	if (GetGameObject()->HasComponent<LevelComponent>())
	{
		m_LevelComponent = GetGameObject()->GetComponent<LevelComponent>();
	}
}
