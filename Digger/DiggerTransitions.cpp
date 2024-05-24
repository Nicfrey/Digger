#include "DiggerTransitions.h"

#include <iostream>

#include "Blackboard.h"

bool HasSelectedLevelTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool hasSelectedLevel;
	bool hasSelectedGameMode;
	if(pBlackBoard->GetValue("hasSelectedLevel",hasSelectedLevel))
	{
		if(pBlackBoard->GetValue("hasSelectedGameMode",hasSelectedGameMode))
		{
			return hasSelectedGameMode && hasSelectedLevel;
		}
		std::cerr << "HasSelectedLevelTransition: Could not find hasSelectedGameMode in blackboard\n";
		return false;
	}
	std::cerr << "HasSelectedLevelTransition: Could not find hasSelectedLevel in blackboard\n";
	return false;
}

bool LoadLevelTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if(pBlackBoard->GetValue("hasLoadedLevel",isTransition))
	{
		return isTransition;
	}
	std::cerr << "LoadLevelTransition: Could not find hasLoadedLevel in blackboard\n";
	return false;
}

bool PlayerDeadTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if(pBlackBoard->GetValue("isDead",isTransition))
	{
		return isTransition;
	}
	std::cerr << "PlayerDeadTransition: Could not find isDead in blackboard\n";
	return false;
}

bool PlayerRespawnTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if(pBlackBoard->GetValue("isRespawn",isTransition))
	{
				return isTransition;
	}
	std::cerr << "PlayerRespawnTransition: Could not find isRespawn in blackboard\n";
	return false;
}

bool PlayerWinTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool allEnemyKilled;
	bool allEmeraldCollected;
	if(pBlackBoard->GetValue("allEnemyKilled",allEnemyKilled))
	{
		if(pBlackBoard->GetValue("allEmeraldCollected",allEmeraldCollected))
		{
			return allEnemyKilled || allEmeraldCollected;
		}
		std::cerr << "PlayerWinTransition: Could not find allEmeraldCollected in blackboard\n";
		return false;
	}
	std::cerr << "PlayerWinTransition: Could not find allEnemyKilled in blackboard\n";
	return false;
}

bool GameOverTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	int levelRemaining;
	int lifeRemaining;
	if(pBlackBoard->GetValue("lifeRemaining",lifeRemaining))
	{
		if(pBlackBoard->GetValue("levelRemaining",levelRemaining))
		{
			return lifeRemaining <= 0 || levelRemaining <= 0;
		}
		std::cerr << "GameOverTransition: Could not find levelRemaining in blackboard\n";
		return false;
	}
	std::cerr << "GameOverTransition: Could not find lifeRemaining in blackboard\n";
	return false;
}
