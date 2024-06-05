#include "GameState.h"

#include "FiniteStateMachine.h"

GameState::~GameState()
{
	delete m_StateMachine;
	m_StateMachine = nullptr;
}

void GameState::Update()
{
	m_StateMachine->Update();
}

void GameState::AddState(FSMStateNode* pState) const
{
	m_StateMachine->AddStateNode(pState);
}

void GameState::AddTransition(FSMStateNode* pStartState, FSMStateNode* pEndState, FSMTransition* pCondition) const
{
	m_StateMachine->AddTransition(pStartState, pEndState, pCondition);
}

void GameState::SetStartState(FSMStateNode* pStartState) const
{
	m_StateMachine->SetStartNode(pStartState);
}


