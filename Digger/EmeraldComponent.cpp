#include "EmeraldComponent.h"

#include "GameObject.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "ScoreComponent.h"

EmeraldComponent::EmeraldComponent(): ItemComponent{ItemType::Emerald}
{
}

void EmeraldComponent::Init()
{
	ItemComponent::Init();
}

std::shared_ptr<BaseComponent> EmeraldComponent::Clone() const
{
	return std::make_shared<EmeraldComponent>(*this);
}

void EmeraldComponent::OnCollisionEnter(std::shared_ptr<dae::GameObject>& other)
{
	if(GetGameObject()->IsDestroyed())
	{
		return;
	}
	if(other->HasComponent<PlayerComponent>() && other->HasComponent<ScoreComponent>())
	{
		const auto score{ other->GetComponent<ScoreComponent>() };
		score->AddScore(m_ScoreValue);
		EventManager::GetInstance().NotifyEvent("EmeraldCollected");
		GetGameObject()->Destroy();
	}
}
