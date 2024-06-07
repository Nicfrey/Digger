#include "DiggerTransitionAnim.h"

#include "Blackboard.h"
#include "EnemyComponent.h"
#include "MoneyBagComponent.h"

bool TransitionPlayerNoProjectile::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if (pBlackBoard->GetValue("HasProjectile", isTransition))
	{
		return !isTransition;
	}
	return false;
}

bool TransitionPlayerHasProjectile::CalculateCondition(Blackboard* pBlackBoard) const
{
	return !TransitionPlayerNoProjectile::CalculateCondition(pBlackBoard);
}

bool TransitionPlayerIsDead::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if (pBlackBoard->GetValue("PlayerDied", isTransition))
	{
		return isTransition;
	}
	return false;
}

bool TransitionPlayerIsAlive::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if(pBlackBoard->GetValue("PlayerDied",isTransition))
	{
		return !isTransition;
	}
	return false;
}

bool TransitionMoneyBagCanFall::CalculateCondition(Blackboard* pBlackBoard) const
{
	MoneyBagComponent::StateMoneyBag isTransition;
	if(pBlackBoard->GetValue("MoneyBagState",isTransition))
	{
		return isTransition == MoneyBagComponent::StateMoneyBag::CanFall;
	}
	return false;
}

bool TransitionMoneyBagIsIdle::CalculateCondition(Blackboard* pBlackBoard) const
{
	MoneyBagComponent::StateMoneyBag isTransition;
	if (pBlackBoard->GetValue("MoneyBagState", isTransition))
	{
		return isTransition == MoneyBagComponent::StateMoneyBag::Idle;
	}
	return false;
}

bool TransitionMoneyBagIsFalling::CalculateCondition(Blackboard* pBlackBoard) const
{
	MoneyBagComponent::StateMoneyBag isTransition;
	if (pBlackBoard->GetValue("MoneyBagState", isTransition))
	{
		return isTransition == MoneyBagComponent::StateMoneyBag::IsFalling;
	}
	return false;
}

bool TransitionMoneyBagIsDestroyed::CalculateCondition(Blackboard* pBlackBoard) const
{
	MoneyBagComponent::StateMoneyBag isTransition;
	if (pBlackBoard->GetValue("MoneyBagState", isTransition))
	{
		return isTransition == MoneyBagComponent::StateMoneyBag::IsDestroyed;
	}
	return false;
}

bool TransitionMoneyBagIsDestroyedIdle::CalculateCondition(Blackboard* pBlackBoard) const
{
	MoneyBagComponent::StateMoneyBag isTransition;
	if (pBlackBoard->GetValue("MoneyBagState", isTransition))
	{
		return isTransition == MoneyBagComponent::StateMoneyBag::IdleDestroyed;
	}
	return false;
}

bool TransitionEnemyIsDead::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isDead;
	if(pBlackBoard->GetValue("EnemyIsDead",isDead))
	{
		return isDead;
	}
	return false;
}

bool TransitionEnemyNobbinsTransformed::CalculateCondition(Blackboard* pBlackBoard) const
{
	EnemyComponent::EnemyType isTransformed;
	if (pBlackBoard->GetValue("EnemyType", isTransformed))
	{
		return isTransformed == EnemyComponent::EnemyType::Nobbins;
	}
	return false;
}

bool TransitionEnemyHobbinsTransformed::CalculateCondition(Blackboard* pBlackBoard) const
{
	EnemyComponent::EnemyType isTransformed;
	if (pBlackBoard->GetValue("EnemyType", isTransformed))
	{
		return isTransformed == EnemyComponent::EnemyType::Hobbins;
	}
	return false;
}
