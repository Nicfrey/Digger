#pragma once
#include "FiniteStateMachine.h"

enum class GameMode;

class MenuState : public FSMStateNode
{
public:
	MenuState() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
private:
	void HasSelectedGameMode();
	void CancelGameMode();
	void HasSelectedLevel();
	bool m_HasSelectedGameMode{};
	bool m_HasSelectedLevel{};
};

class LoadState : public FSMStateNode
{
public:
	LoadState() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
private:
	void HasLoadedLevel();
	bool m_HasLoadedLevel{};
};

class PlayState : public FSMStateNode
{
public:
	PlayState() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
private:
	void HandlePlayerDead();
	void SetPlayerIsDead();
	void HandlePlayerWon();
	void SetPlayerWon();
	bool m_PlayerIsDead{false};
	bool m_PlayerHasWon{false};
};

class RespawnState : public FSMStateNode
{
public:
	RespawnState() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
};

class GameOverState : public FSMStateNode
{
public:
	GameOverState() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
};

class WinState : public FSMStateNode
{
public:
	WinState() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
};

