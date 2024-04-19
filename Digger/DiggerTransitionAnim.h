#pragma once
#include "FiniteStateMachine.h"

class TransitionNoProjectile : public FSMTransition
{
public:
	TransitionNoProjectile() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionNoProjectile() override = default;
};

class TransitionProjectile : public TransitionNoProjectile
{
public:
	TransitionProjectile() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionProjectile() override = default;
};

class TransitionDead : public FSMTransition
{
public:
	TransitionDead() = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
	~TransitionDead() override = default;
};

