#pragma once
#include "BaseComponent.h"

class FSMTransition;
class FSMStateNode;
class Blackboard;
class FiniteStateMachine;

class GameState : public BaseComponent
{
public:
	GameState(Blackboard* pBlackboard);
	~GameState() override;
	GameState(const GameState& other);
	GameState(GameState&& other) noexcept;
	GameState& operator=(const GameState& other);
	GameState& operator=(GameState&& other) noexcept;

	std::shared_ptr<BaseComponent> Clone() const override;
	void Update() override;
	void AddState(FSMStateNode* pState) const;
	void AddTransition(FSMStateNode* pStartState, FSMStateNode* pEndState, FSMTransition* pCondition) const;
	void SetStartState(FSMStateNode* pStartState) const;
private:
	FiniteStateMachine* m_StateMachine;
};
