#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>
class Blackboard;

/**
 * \brief Abstract class for a transition in a finite state machine
 * To use it, derive from this class and implement the CalculateCondition function
 */
class FSMTransition
{
public:
	FSMTransition() = default;
	virtual ~FSMTransition() = default;
	FSMTransition(const FSMTransition& other) = default;
	FSMTransition(FSMTransition&& other) = default;
	FSMTransition& operator=(const FSMTransition& other) = default;
	FSMTransition& operator=(FSMTransition&& other) = default;
	virtual bool CalculateCondition(Blackboard* pBlackBoard) const = 0;
};

/**
 * \brief Abstract class for a state in a finite state machine
 * To use it, derive from this class and implement the Enter, Update and Exit functions
 */
class FSMStateNode
{
public:
	FSMStateNode() = default;
	virtual ~FSMStateNode() = default;
	FSMStateNode(const FSMStateNode& other) = delete;
	FSMStateNode(FSMStateNode&& other) noexcept = delete;
	FSMStateNode& operator=(const FSMStateNode& other) = delete;
	FSMStateNode& operator=(FSMStateNode&& other) noexcept = delete;

	virtual void Enter(Blackboard* pBlackboard) = 0;
	virtual void Update(Blackboard* pBlackboard) = 0;
	virtual void Exit(Blackboard* pBlackboard) = 0;
};

/**
 * \brief Finite State Machine for AI
 */
class FiniteStateMachine
{
	typedef std::pair<FSMTransition*, FSMStateNode*> TransitionStatePair;
	typedef std::vector<TransitionStatePair> Transitions;
public:
	FiniteStateMachine(Blackboard* pBlackboard);
	FiniteStateMachine(FSMStateNode* startNode,Blackboard* pBlackboard);
	virtual ~FiniteStateMachine();
	void SetStartNode(FSMStateNode* startNode);
	void AddStateNode(FSMStateNode* stateNode);
	void AddTransition(FSMStateNode* startNode, FSMStateNode* endNode, FSMTransition* condition);
	FSMStateNode* GetCurrentState() const { return m_pCurrentState; };
	void Update();
	Blackboard* GetBlackboard() const;
protected:
	void ChangeState(FSMStateNode* newState);

	std::map<FSMStateNode*,Transitions> m_Transitions;
	FSMStateNode* m_pCurrentState;
	Blackboard* m_pBlackboard{nullptr};
};
