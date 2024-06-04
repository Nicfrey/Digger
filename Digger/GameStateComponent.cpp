#include "GameStateComponent.h"

#include "Blackboard.h"
#include "DiggerStates.h"
#include "DiggerTransitions.h"
#include "PlayerComponent.h"
#include "Scene.h"
#include "SceneManager.h"

GameStateComponent::GameStateComponent() : BaseComponent(nullptr), m_StateMachine(std::make_unique<DiggerStateMachine>(new Blackboard{}))
{
	const auto menuState{ new MenuState{} };
	const auto loadState{ new LoadState{} };
	const auto playState{ new PlayState{} };
	const auto gameOverState{ new GameOverState() };
	const auto respawnState{ new RespawnState() };
	m_StateMachine->AddStateNode(menuState);
	m_StateMachine->AddStateNode(loadState);
	m_StateMachine->AddStateNode(playState);
	m_StateMachine->AddStateNode(gameOverState);
	m_StateMachine->AddStateNode(respawnState);
	const auto selectedLevelTrans{ new HasSelectedLevelTransition{} };
	const auto loadLevelTransition{ new LoadLevelTransition{} };
	const auto playerDeadTransition{ new PlayerDeadTransition{} };
	const auto respawnTransition{ new PlayerRespawnTransition{} };
	const auto gameOverTransition{ new GameOverTransition{} };
	const auto wroteHighScoreTransition{ new WroteHighScoreTransition{} };
	m_StateMachine->AddTransition(menuState, loadState, selectedLevelTrans);
	m_StateMachine->AddTransition(loadState, playState, loadLevelTransition);
	m_StateMachine->AddTransition(playState, respawnState, playerDeadTransition);
	m_StateMachine->AddTransition(respawnState, playState, respawnTransition);
	m_StateMachine->AddTransition(playState, gameOverState, gameOverTransition);
	m_StateMachine->AddTransition(gameOverState, menuState, wroteHighScoreTransition);
	m_StateMachine->GetBlackboard()->AddValue("hasSelectedLevel", false);
	m_StateMachine->GetBlackboard()->AddValue("hasSelectedGameMode", false);
	m_StateMachine->GetBlackboard()->AddValue("hasLoadedLevel", false);
	m_StateMachine->GetBlackboard()->AddValue("currentLevel", 0);
	m_StateMachine->GetBlackboard()->AddValue("hasLoadedLevel", false);
	m_StateMachine->GetBlackboard()->AddValue("isPlayerDead", false);
	m_StateMachine->GetBlackboard()->AddValue("hasWrittenHighScore", false);
	m_StateMachine->GetBlackboard()->AddValue("hasExtraLife", false);
	m_StateMachine->GetBlackboard()->AddValue("levelRemaining", 2);
	m_StateMachine->GetBlackboard()->AddValue("players", dae::SceneManager::GetInstance().GetGameObjectsWithComponent<PlayerComponent>());
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
