#include "MoneyBagComponent.h"

#include "AnimatorComponent.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "TimeEngine.h"

MoneyBagComponent::MoneyBagComponent(const MoneyBagComponent& other): BaseComponent{other}, m_State{other.m_State}
{
}

MoneyBagComponent::MoneyBagComponent(MoneyBagComponent&& other) noexcept: BaseComponent(std::move(other)), m_State{std::move(other.m_State)}
{
}

MoneyBagComponent& MoneyBagComponent::operator=(const MoneyBagComponent& other)
{
	if(this != &other)
	{
		return *this;
	}
	BaseComponent::operator=(other);
	m_State = other.m_State;
	return *this;
}

MoneyBagComponent& MoneyBagComponent::operator=(MoneyBagComponent&& other) noexcept
{
	if (this != &other)
	{
		return *this;
	}
	BaseComponent::operator=(std::move(other));
	m_State = std::move(other.m_State);
	return *this;
}

std::shared_ptr<BaseComponent> MoneyBagComponent::Clone() const
{
	return std::make_shared<MoneyBagComponent>(*this);
}

void MoneyBagComponent::Init()
{
	m_State = StateMoneyBag::Idle;
	if(GetGameObject()->HasComponent<AnimatorComponent>())
	{
		const auto animator{ GetGameObject()->GetComponent<AnimatorComponent>() };
		animator->AddParameter("MoneyBagState",m_State);
	}
}

void MoneyBagComponent::Update()
{
	if(m_State == StateMoneyBag::IsFalling)
	{
		// Add velocity to money bag
		GetGameObject()->Translate(m_Velocity * TimeEngine::GetInstance().GetDeltaTime());
	}
	if(m_State == StateMoneyBag::CanFall)
	{
		// Increment timer about to fall
	}
}

void MoneyBagComponent::OnCollisionEnter(std::shared_ptr<dae::GameObject>& other)
{
	switch (m_State)
	{
	case StateMoneyBag::Idle:
	case StateMoneyBag::CanFall:
		break;
	case StateMoneyBag::IsFalling:
		if(other->HasComponent<EnemyComponent>())
		{
			// TODO Set the animation of enemy to dead
			// TODO Add sound for killing enemy
			auto goPlayer{ dae::SceneManager::GetInstance().GetGameObjectByTag("Player") };
			if(goPlayer->HasComponent<ScoreComponent>())
			{
				// TODO Add score of enemy to player
			}
		}
		break;
	case StateMoneyBag::IsDestroyed:
	case StateMoneyBag::IdleDestroyed:
		if (other->HasComponent<PlayerComponent>())
		{
			const auto scorePlayer{ other->GetComponent<ScoreComponent>() };
			scorePlayer->AddScore(m_Score);
			// TODO Add sound for getting bag
			// TODO Update UI
			GetGameObject()->Destroy();
		}
		break;
	}
}
