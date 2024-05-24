#include "BackgroundComponent.h"

#include "GameObject.h"
#include "PlayerComponent.h"
#include "SceneManager.h"

std::shared_ptr<BaseComponent> BackgroundComponent::Clone() const
{
	return std::make_shared<BackgroundComponent>(*this);
}

void BackgroundComponent::OnCollisionEnter(std::shared_ptr<dae::GameObject>& other)
{
	if(other->HasComponent<PlayerComponent>())
	{
		GetGameObject()->Destroy();
	}
}
