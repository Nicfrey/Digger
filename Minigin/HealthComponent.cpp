#include "HealthComponent.h"

#include "Observer.h"

HealthComponent::HealthComponent(): BaseComponent{nullptr}, m_LifeRemaining{3}
{

}


void HealthComponent::LoseOneLife()
{
	--m_LifeRemaining;
	if(m_LifeRemaining < 0)
	{
		m_LifeRemaining = 0;
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
	return m_LifeRemaining == 0;
}

void HealthComponent::Update()
{
}

void HealthComponent::FixedUpdate()
{
}

void HealthComponent::Init()
{

}

void HealthComponent::RenderGUI()
{
}
