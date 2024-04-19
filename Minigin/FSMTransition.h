#pragma once
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

