#include "GameStateComponent.h"

#include "Blackboard.h"
#include "DiggerStates.h"
#include "DiggerTransitions.h"

GameStateComponent::GameStateComponent() : BaseComponent(nullptr), m_StateMachine(std::make_unique<DiggerStateMachine>(new Blackboard{}))
{
	const auto menuState{ new MenuState{} };
	const auto loadState{ new LoadState{} };
	const auto playState{ new PlayState{} };
	m_StateMachine->AddStateNode(menuState);
	m_StateMachine->AddStateNode(loadState);
	m_StateMachine->AddStateNode(playState);
	const auto selectedLevelTrans{ new HasSelectedLevelTransition{} };
	const auto loadLevelTransition{ new LoadLevelTransition{} };
	m_StateMachine->AddTransition(menuState, loadState, selectedLevelTrans);
	m_StateMachine->AddTransition(loadState, playState, loadLevelTransition);
	m_StateMachine->GetBlackboard()->AddValue("hasSelectedLevel", false);
	m_StateMachine->GetBlackboard()->AddValue("hasSelectedGameMode", false);
	m_StateMachine->GetBlackboard()->AddValue("hasLoadedLevel", false);
	m_StateMachine->GetBlackboard()->AddValue("currentLevel", 0);
	m_StateMachine->SetStartNode(menuState);
}

GameStateComponent::GameStateComponent(const GameStateComponent& other): BaseComponent(other)
{
}

GameStateComponent::GameStateComponent(GameStateComponent&& other) noexcept: BaseComponent(std::move(other))
{
}

GameStateComponent& GameStateComponent::operator=(const GameStateComponent& other)
{
	if(&other == this)
	{
		return *this;
	}
	BaseComponent::operator=(other);
	return *this;
}

GameStateComponent& GameStateComponent::operator=(GameStateComponent&& other) noexcept
{
	if (&other == this)
	{
		return *this;
	}
	BaseComponent::operator=(std::move(other));
	return *this;
}

void GameStateComponent::Update()
{
	m_StateMachine->Update();
}

std::shared_ptr<BaseComponent> GameStateComponent::Clone() const
{
	return std::make_shared<GameStateComponent>(*this);
}
