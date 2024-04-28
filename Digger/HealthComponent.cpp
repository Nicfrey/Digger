#include "HealthComponent.h"

#include "GameObject.h"
#include "Observer.h"
#include "PlayerComponent.h"

HealthComponent::HealthComponent(): BaseComponent{nullptr}, m_LifeRemaining{3}
{

}

HealthComponent::HealthComponent(int lifeRemaining): m_LifeRemaining{lifeRemaining}
{
}

std::shared_ptr<BaseComponent> HealthComponent::Clone() const
{
	return std::make_shared<HealthComponent>(*this);
}


void HealthComponent::LoseOneLife()
{
	--m_LifeRemaining;
	if(IsDead())
	{
		if(GetGameObject()->HasComponent<PlayerComponent>())
		{
			EventManager::GetInstance().NotifyEvent("PlayerDied");
		}
		else
		{
			EventManager::GetInstance().NotifyEvent("EnemyDied");
		}
	}
	EventManager::GetInstance().NotifyEvent("LifeLost");
}

void HealthComponent::GainOneLife()
{
	++m_LifeRemaining;
	EventManager::GetInstance().NotifyEvent("LifeGained");
}

int HealthComponent::GetLifeRemaining() const
{
	return m_LifeRemaining;
}

bool HealthComponent::IsDead() const
{
	return m_LifeRemaining <= 0;
}

