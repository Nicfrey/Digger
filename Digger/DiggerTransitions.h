#pragma once
#include "DiggerTransitionAnim.h"

class HasSelectedLevelTransition final : public FSMTransition
{
public:
	HasSelectedLevelTransition() = default;
	~HasSelectedLevelTransition() override = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
};

class LoadLevelTransition final : public FSMTransition
{
public:
	LoadLevelTransition() = default;
	~LoadLevelTransition() override = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
};

class PlayerDeadTransition final : public FSMTransition
{
public:
	PlayerDeadTransition() = default;
	~PlayerDeadTransition() override = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
};

class PlayerRespawnTransition final : public FSMTransition
{
public:
	PlayerRespawnTransition() = default;
	~PlayerRespawnTransition() override = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
};

class PlayerWinTransition final : public FSMTransition
{
public:
	PlayerWinTransition() = default;
	~PlayerWinTransition() override = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
};

class GameOverTransition final : public FSMTransition
{
public:
	GameOverTransition() = default;
	~GameOverTransition() override = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
};

class WroteHighScoreTransition final : public FSMTransition
{
public:
	WroteHighScoreTransition() = default;
	~WroteHighScoreTransition() override = default;
	bool CalculateCondition(Blackboard* pBlackBoard) const override;
};



