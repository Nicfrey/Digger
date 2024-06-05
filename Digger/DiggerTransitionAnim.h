#pragma once
#include "FiniteStateMachine.h"
class TransitionPlayerNoProjectile : public FSMTransition
{
public:
	TransitionPlayerNoProjectile() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionPlayerNoProjectile() override = default;
};

class TransitionPlayerHasProjectile : public TransitionPlayerNoProjectile
{
public:
	TransitionPlayerHasProjectile() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionPlayerHasProjectile() override = default;
};

class TransitionPlayerIsDead : public FSMTransition
{
public:
	TransitionPlayerIsDead() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionPlayerIsDead() override = default;
};

class TransitionPlayerIsAlive : public FSMTransition
{
public:
	TransitionPlayerIsAlive() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionPlayerIsAlive() override = default;
};

class TransitionMoneyBagCanFall : public FSMTransition
{
public:
	TransitionMoneyBagCanFall() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionMoneyBagCanFall() override = default;
};

class TransitionMoneyBagIsIdle : public FSMTransition
{
public:
	TransitionMoneyBagIsIdle() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionMoneyBagIsIdle() override = default;
};

class TransitionMoneyBagIsDestroyed : public FSMTransition
{
public:
	TransitionMoneyBagIsDestroyed() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionMoneyBagIsDestroyed() override = default;
};

class TransitionMoneyBagIsDestroyedIdle : public FSMTransition
{
public:
	TransitionMoneyBagIsDestroyedIdle() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionMoneyBagIsDestroyedIdle() override = default;
};

class TransitionEnemyIsDead : public FSMTransition
{
public:
	TransitionEnemyIsDead() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionEnemyIsDead() override = default;
};

