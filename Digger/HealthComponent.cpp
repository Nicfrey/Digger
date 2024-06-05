#include "HealthComponent.h"

#include "EnemyComponent.h"
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
	m_Alive = false;
	--m_LifeRemaining;
	if(IsDead())
	{
		if(GetGameObject()->HasComponent<PlayerComponent>())
		{
			EventManager::GetInstance().NotifyEvent("PlayerDied");
		}
		if(GetGameObject()->HasComponent<EnemyComponent>())
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

bool HealthComponent::HasNoRemainingLife() const
{
	return m_LifeRemaining <= 0;
}

bool HealthComponent::IsDead() const
{
	return !m_Alive;
}

void HealthComponent::OnDestroy()
{

}

void HealthComponent::SetAlive()
{
	m_Alive = true;
}

