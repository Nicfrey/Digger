#pragma once
class Blackboard;

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

