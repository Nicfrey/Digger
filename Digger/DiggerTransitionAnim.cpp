#include "DiggerTransitionAnim.h"

#include "Blackboard.h"
#include "GameObject.h"

bool TransitionNoProjectile::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if (pBlackBoard->GetValue("HasProjectile", isTransition))
	{
		return !isTransition;
	}
	return false;
}

bool TransitionProjectile::CalculateCondition(Blackboard* pBlackBoard) const
{
	return !TransitionNoProjectile::CalculateCondition(pBlackBoard);
}

bool TransitionDead::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if (pBlackBoard->GetValue("PlayerDied", isTransition))
	{
		return isTransition;
	}
	return false;
}
