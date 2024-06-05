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
	if(pBlackBoard->GetValue("isPlayerDead",isTransition))
	{
		return isTransition;
	}
	std::cerr << "PlayerDeadTransition: Could not find isPlayerDead in blackboard\n";
	return false;
}

bool PlayerRespawnTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if(pBlackBoard->GetValue("hasLoadedLevel",isTransition))
	{
		return isTransition;
	}
	std::cerr << "PlayerRespawnTransition: Could not find hasLoadedLevel in blackboard\n";
	return false;
}

bool PlayerWinTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool hasPlayerWon;
	if(pBlackBoard->GetValue("hasPlayerWon",hasPlayerWon))
	{
		return hasPlayerWon;
	}
	return false;
}

bool SkipLevelTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if (pBlackBoard->GetValue("hasSkippedLevel", isTransition))
	{
		return isTransition;
	}
	std::cerr << "SkipLevelTransition: Could not find hasSkippedLevel in blackboard\n";
	return false;
}

bool GameOverTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool lifeRemaining;
	int levelRemaining;
	if(pBlackBoard->GetValue("hasExtraLife",lifeRemaining))
	{
		if(pBlackBoard->GetValue("levelRemaining",levelRemaining))
		{
			return !lifeRemaining || levelRemaining <= 0;
		}
		std::cerr << "GameOverTransition: Could not find levelRemaining in blackboard\n";
		return false;
	}
	std::cerr << "GameOverTransition: Could not find hasExtraLife in blackboard\n";
	return false;
}

bool WroteHighScoreTransition::CalculateCondition(Blackboard* pBlackBoard) const
{
	bool isTransition;
	if(pBlackBoard->GetValue("wroteHighScore",isTransition))
	{
		return isTransition;
	}
	std::cerr << "WroteHighScoreTransition: Could not find wroteHighScore in blackboard\n";
	return false;
}
