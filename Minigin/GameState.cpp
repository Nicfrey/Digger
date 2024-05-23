#include "GameState.h"

#include "FiniteStateMachine.h"

GameState::GameState(Blackboard* pBlackboard): BaseComponent{nullptr}
{
	m_StateMachine = new FiniteStateMachine(pBlackboard);
}

GameState::~GameState()
{
	delete m_StateMachine;
	m_StateMachine = nullptr;
}

GameState::GameState(const GameState& other): BaseComponent(other)
{
	m_StateMachine = new FiniteStateMachine(*other.m_StateMachine);
}

GameState::GameState(GameState&& other) noexcept: BaseComponent(std::move(other)), m_StateMachine{std::move(other.m_StateMachine)}
{
	other.m_StateMachine = nullptr;
}

GameState& GameState::operator=(const GameState& other)
{
	if(this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(other);
	delete m_StateMachine;
	m_StateMachine = new FiniteStateMachine(*other.m_StateMachine);
	return *this;
}

GameState& GameState::operator=(GameState&& other) noexcept
{
		if(this == &other)
		{
					return *this;
	}
	BaseComponent::operator=(std::move(other));
	delete m_StateMachine;
	m_StateMachine = std::move(other.m_StateMachine);
	other.m_StateMachine = nullptr;
	return *this;
}

std::shared_ptr<BaseComponent> GameState::Clone() const
{
	return std::make_shared<GameState>(*this);
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


