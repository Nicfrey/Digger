#pragma once
#include <glm/vec3.hpp>

#include "FiniteStateMachine.h"
#include "MoneyBagComponent.h"

#pragma region GameStates
namespace GraphUtils
{
	class GraphNode;
}

class MoneyBagComponent;
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
	void SkipLevel();
	bool m_PlayerIsDead{false};
	bool m_PlayerHasWon{false};
	bool m_HasSkippedLevel{ false };
	std::vector<std::shared_ptr<dae::GameObject>> m_Players;
};

class RespawnState : public FSMStateNode
{
public:
	RespawnState() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
private:
	bool m_HasLoaded;
	void HasLoadedLevel();
};

class GameOverState : public FSMStateNode
{
public:
	GameOverState() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
private:
	bool m_HasSetName;
	void HasSetName();
};
#pragma endregion GameStates

#pragma region MoneyBagStates

class IdleStateMoneyBag : public FSMStateNode
{
public:
	IdleStateMoneyBag() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
private:
	MoneyBagComponent::StateMoneyBag m_State;
	glm::vec3 m_Position;
	void SetFallingState();
};

class FallingStateMoneyBag : public FSMStateNode
{
public:
	FallingStateMoneyBag() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
private:
	void HandleMoneyBagLanded();
	MoneyBagComponent::StateMoneyBag m_State{};
	std::vector<GraphUtils::GraphNode*> m_NodeTravelled{};
	std::shared_ptr<dae::GameObject> m_MoneyBagObject{nullptr};
	float m_FallSpeed{80.f};
	glm::vec2 m_Direction{ 0,1 };
};

#pragma endregion MoneyBagState

#pragma region PlayerStates

class IdleWalkStatePlayer : public FSMStateNode
{
public:
	IdleWalkStatePlayer() = default;
	void Enter(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
};

#pragma endregion PlayerStates