#pragma once
#include "BaseComponent.h"
#include "Blackboard.h"
#include "FiniteStateMachine.h"
#include "Singleton.h"

class FSMTransition;
class FSMStateNode;
class Blackboard;
class FiniteStateMachine;

class GameState final : public dae::Singleton<GameState>
{
public:
	GameState() = default;
	~GameState() override;
	GameState(const GameState& other) = delete;
	GameState(GameState&& other) noexcept = delete;
	GameState& operator=(const GameState& other) = delete;
	GameState& operator=(GameState&& other) noexcept = delete;

	void Update();
	template<typename T>
	void AddParameter(const std::string& nameParam, T value);
	void AddState(FSMStateNode* pState) const;
	void AddTransition(FSMStateNode* pStartState, FSMStateNode* pEndState, FSMTransition* pCondition) const;
	void SetStartState(FSMStateNode* pStartState) const;
private:
	FiniteStateMachine* m_StateMachine{new FiniteStateMachine{new Blackboard{}}};
};

template <typename T>
void GameState::AddParameter(const std::string& nameParam, T value)
{
	m_StateMachine->GetBlackboard()->AddValue(nameParam, value);
}
