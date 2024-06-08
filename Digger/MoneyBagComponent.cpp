#include "MoneyBagComponent.h"

#include "AnimatorComponent.h"
#include "BackgroundComponent.h"
#include "DiggerStates.h"
#include "DiggerTransitionAnim.h"
#include "DiggerUtils.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "PlayerComponent.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "TimeEngine.h"
#include "LevelComponent.h"
#include "Observer.h"
#include "Scene.h"
#include "SoundSystemEngine.h"

MoneyBagComponent::StateMoneyBag MoneyBagComponent::GetState() const
{
	return m_State;
}

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
	ConfigureStateMachine();
}

void MoneyBagComponent::Update()
{
	m_MoneyBagStates->Update();
	m_MoneyBagStates->GetBlackboard()->GetValue<StateMoneyBag>("MoneyBagState",m_State);
	GetGameObject()->GetComponent<AnimatorComponent>()->SetParameter("MoneyBagState", m_State);
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
			if(!other->GetComponent<HealthComponent>()->IsDead())
			{
				auto goPlayer{ dae::SceneManager::GetInstance().GetGameObjectWithComponent<PlayerComponent>() };
				if (goPlayer->HasComponent<ScoreComponent>())
				{
					goPlayer->GetComponent<ScoreComponent>()->AddScore(250);
				}
				other->GetComponent<HealthComponent>()->LoseOneLife();
			}
		}
		if(other->HasComponent<BackgroundComponent>())
		{
			EventManager::GetInstance().NotifyEvent("MoneyBagLanded");
		}
		if (other->HasComponent<PlayerComponent>())
		{
			const auto health{ other->GetComponent<HealthComponent>() };
			health->LoseOneLife();
			ServiceSoundLocator::GetSoundSystem().Play(TO_SOUND_ID(DiggerUtils::SoundDiggerID::PROJECTILE_HIT_ENTITIES), 50);
			EventManager::GetInstance().NotifyEvent("PlayerDied");
		}
		break;
	case StateMoneyBag::IsDestroyed:
	case StateMoneyBag::IdleDestroyed:
		if (other->HasComponent<PlayerComponent>())
		{
			const auto scorePlayer{ other->GetComponent<ScoreComponent>() };
			scorePlayer->AddScore(m_Score);
			ServiceSoundLocator::GetSoundSystem().Play(TO_SOUND_ID(DiggerUtils::SoundDiggerID::COLLECT_MONEY_BAG), 50);
			GetGameObject()->Destroy();
		}
		break;
	}
}

void MoneyBagComponent::ConfigureStateMachine()
{
	Blackboard* pBlackboard{ new Blackboard{} };
	pBlackboard->AddValue("MoneyBagState", m_State);
	pBlackboard->AddValue("Position", GetGameObject()->GetWorldPosition());
	pBlackboard->AddValue("MoneyBagObject", GetGameObject()->GetThis());
	m_MoneyBagStates = std::make_unique<MoneyBagStateMachine>(pBlackboard);
	FallingStateMoneyBag* pFallingState{ new FallingStateMoneyBag{} };
	IdleStateMoneyBag* pIdleState{ new IdleStateMoneyBag{} };
	TransitionMoneyBagIsDestroyed* pTransitionIsDestroyed{ new TransitionMoneyBagIsDestroyed{} };
	TransitionMoneyBagIsFalling* pTransitionIsFalling{ new TransitionMoneyBagIsFalling{} };
	TransitionMoneyBagIsIdle* pTransitionIsIdle{ new TransitionMoneyBagIsIdle{} };
	m_MoneyBagStates->AddTransition(pIdleState,pFallingState,pTransitionIsFalling);
	m_MoneyBagStates->AddTransition(pFallingState, pIdleState, pTransitionIsDestroyed);
	m_MoneyBagStates->AddTransition(pFallingState, pIdleState, pTransitionIsIdle);
	m_MoneyBagStates->SetStartNode(pIdleState);
}
