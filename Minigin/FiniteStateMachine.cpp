#include "FiniteStateMachine.h"

#include "Blackboard.h"

FiniteStateMachine::FiniteStateMachine(Blackboard* pBlackboard): m_pCurrentState{nullptr}, m_pBlackboard{pBlackboard}
{
}

FiniteStateMachine::FiniteStateMachine(FSMStateNode* startNode, Blackboard* pBlackboard): m_pBlackboard(pBlackboard)
{
	ChangeState(startNode);
}

FiniteStateMachine::~FiniteStateMachine()
{
	std::list<FSMStateNode*> nodesToDelete{};
	std::list<FSMTransition*> transitionsToDelete{};
	for(const auto& state : m_Transitions)
	{
		for (auto transition : state.second)
		{
			auto itTransition = std::ranges::find_if(transitionsToDelete, [transition](const FSMTransition* other)
			{
				return other == transition.first;
			});
			if(itTransition == transitionsToDelete.end())
			{
				transitionsToDelete.push_back(transition.first);
			}
			auto itNode = std::ranges::find_if(nodesToDelete, [transition](const FSMStateNode* other)
			{
				return other == transition.second;
			});
			if(itNode == nodesToDelete.end())
			{
				nodesToDelete.push_back(transition.second);
			}
		}
	}
	for(auto transition : transitionsToDelete)
	{
		delete transition;
		transition = nullptr;
	}
	for(auto node : nodesToDelete)
	{
		delete node;
		node = nullptr;
	}
	delete m_pBlackboard;
}

void FiniteStateMachine::SetStartNode(FSMStateNode* startNode)
{
	ChangeState(startNode);
}

void FiniteStateMachine::AddStateNode(FSMStateNode* stateNode)
{
	const auto it{ m_Transitions.find(stateNode) };
	if (it == m_Transitions.end())
	{
		m_Transitions[stateNode] = Transitions{};
	}
}

void FiniteStateMachine::AddTransition(FSMStateNode* startNode, FSMStateNode* endNode, FSMTransition* condition)
{
	const auto it{ m_Transitions.find(startNode) };
	if(it == m_Transitions.end())
	{
		m_Transitions[startNode] = Transitions{};
	}
	m_Transitions[startNode].emplace_back(condition, endNode);
}

void FiniteStateMachine::Update()
{
	const auto& currentTransition{ m_Transitions.find(m_pCurrentState) };
	if(currentTransition != m_Transitions.end())
	{
		for(const auto& transition : currentTransition->second)
		{
			if(transition.first->CalculateCondition(m_pBlackboard))
			{
				ChangeState(transition.second);
				break;
			}
		}
	}

	if(m_pCurrentState != nullptr)
	{
		m_pCurrentState->Update(m_pBlackboard);
	}
}

Blackboard* FiniteStateMachine::GetBlackboard() const
{
	return m_pBlackboard;
}

void FiniteStateMachine::ChangeState(FSMStateNode* newState)
{
	if(m_pCurrentState != nullptr)
	{
		m_pCurrentState->Exit(m_pBlackboard);
	}
	m_pCurrentState = newState;
	m_pCurrentState->Enter(m_pBlackboard);
}
