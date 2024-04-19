#pragma once
#include <map>
#include <utility>
#include <vector>
class FSMTransition;
class Blackboard;
class FSMStateNode;

/**
 * \brief Finite State Machine for AI and Animation
 */
class FiniteStateMachine final
{
public:
	FiniteStateMachine(FSMStateNode* startNode,Blackboard* pBlackboard);
	~FiniteStateMachine();
	void AddTransition(FSMStateNode* startNode, FSMStateNode* endNode, FSMTransition* condition);
	void Update();
	Blackboard* GetBlackboard() const { return m_pBlackboard; }
private:
	void ChangeState(FSMStateNode* newState);
	typedef std::pair<FSMTransition*, FSMStateNode*> TranstionStatePair;
	typedef std::vector<TranstionStatePair> Transitions;

	std::map<FSMStateNode*,Transitions> m_Transitions;
	FSMStateNode* m_pCurrentState;
	Blackboard* m_pBlackboard{nullptr};
};

