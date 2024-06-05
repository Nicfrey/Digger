#include "DiggerStates.h"

#include <iostream>

#include "Blackboard.h"
#include "DiggerUtils.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "Observer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SoundSystemEngine.h"
#include "Utils.h"
#include "WidgetManager.h"
#include "Graph.h"
#include "HealthComponent.h"
#include "PlayerComponent.h"

void MenuState::Enter(Blackboard* pBlackboard)
{
	// TODO Play menu music
	// Add function to event
	EventManager::GetInstance().AddEvent("SelectGameMode", this, &MenuState::HasSelectedGameMode);
	EventManager::GetInstance().AddEvent("CancelGameMode", this, &MenuState::CancelGameMode);
	EventManager::GetInstance().AddEvent("SelectLevel", this, &MenuState::HasSelectedLevel);
	// Change the state of load level
	pBlackboard->ChangeValue("hasSelectedLevel", false);
}

void MenuState::Update(Blackboard* pBlackboard)
{
	// TODO Check if player has selected a game mode and a level
	if (m_HasSelectedGameMode && m_HasSelectedLevel)
	{
		// Transition to LoadState
		pBlackboard->ChangeValue("hasSelectedLevel", m_HasSelectedLevel);
		pBlackboard->ChangeValue("hasSelectedGameMode", m_HasSelectedGameMode);
	}
}

void MenuState::Exit(Blackboard* pBlackboard)
{
	// TODO Stop menu music
	EventManager::GetInstance().RemoveEvent("SelectGameMode", this, &MenuState::HasSelectedGameMode);
	EventManager::GetInstance().RemoveEvent("CancelGameMode", this, &MenuState::CancelGameMode);
	EventManager::GetInstance().RemoveEvent("SelectLevel", this, &MenuState::HasSelectedLevel);
	m_HasSelectedLevel = false;
	m_HasSelectedGameMode = false;
	pBlackboard->ChangeValue("hasSelectedLevel", m_HasSelectedLevel);
	pBlackboard->ChangeValue("hasSelectedGameMode", m_HasSelectedGameMode);
}

void MenuState::HasSelectedGameMode()
{
	m_HasSelectedGameMode = true;
	WidgetManager::GetInstance().SetActiveWidget("LevelMenu");
}

void MenuState::CancelGameMode()
{
	m_HasSelectedGameMode = false;
	WidgetManager::GetInstance().SetActiveWidget("MainMenu");
}

void MenuState::HasSelectedLevel()
{
	m_HasSelectedLevel = true;
	WidgetManager::GetInstance().RemoveActiveWidget();
}

void LoadState::Enter(Blackboard* pBlackboard)
{
	// Add function to event
	EventManager::GetInstance().AddEvent("LevelLoaded", this, &LoadState::HasLoadedLevel);
	// Change the state of load level
	pBlackboard->ChangeValue("hasLoadedLevel", false);
	EventManager::GetInstance().NotifyEvent("LoadLevel");
}

void LoadState::Update(Blackboard* pBlackboard)
{
	if (m_HasLoadedLevel)
	{
		// Transition to PlayState
		pBlackboard->ChangeValue("hasLoadedLevel", m_HasLoadedLevel);
	}
}

void LoadState::Exit(Blackboard* pBlackboard)
{
	EventManager::GetInstance().RemoveEvent("LevelLoaded", this, &LoadState::HasLoadedLevel);
	pBlackboard->ChangeValue("hasLoadedLevel", false);
	WidgetManager::GetInstance().RemoveActiveWidget();
	m_HasLoadedLevel = false;
}

void LoadState::HasLoadedLevel()
{
	m_HasLoadedLevel = true;
}

void PlayState::Enter(Blackboard* pBlackboard)
{
	m_Players = dae::SceneManager::GetInstance().GetGameObjectsWithComponent<PlayerComponent>();
	pBlackboard->ChangeValue("isPlayerDead", false);
	pBlackboard->ChangeValue("hasPlayerWon", false);
	pBlackboard->ChangeValue("hasExtraLife", true);
	EventManager::GetInstance().AddEvent("PlayerDied", this, &PlayState::HandlePlayerDead);
	EventManager::GetInstance().AddEvent("PlayerWon", this, &PlayState::HandlePlayerWon);
	// TODO Play music of the game
	ServiceMusicLocator::GetMusicSystem().Play(static_cast<MusicId>(DiggerUtils::MusicDiggerID::GAME), true);
}

void PlayState::Update(Blackboard* pBlackboard)
{
	if (m_PlayerIsDead)
	{
		pBlackboard->ChangeValue("isPlayerDead", true);
		if (m_Players[0]->GetComponent<HealthComponent>()->HasNoRemainingLife())
		{
			pBlackboard->ChangeValue("hasNoExtraLife", true);
		}
		return;
	}
	if (m_PlayerHasWon)
	{
		pBlackboard->ChangeValue("hasPlayerWon", true);
	}
}

void PlayState::HandlePlayerDead()
{
	// Check if both players are dead
	size_t indexDead{};
	for (const auto& player : m_Players)
	{
		if (player->GetComponent<HealthComponent>()->IsDead())
		{
			++indexDead;
		}
	}
	if(indexDead == m_Players.size())
	{
		TimerManager::GetInstance().AddTimer(this, &PlayState::SetPlayerIsDead, 5.f);
		ServiceMusicLocator::GetMusicSystem().Play(static_cast<MusicId>(DiggerUtils::MusicDiggerID::PLAYER_DIED), false);
	}
}

void PlayState::SetPlayerIsDead()
{
	m_PlayerIsDead = true;
}

void PlayState::HandlePlayerWon()
{
	// Wait 5 sec before setting the player to won
	TimerManager::GetInstance().AddTimer(this, &PlayState::SetPlayerWon, 5.f);
	ServiceMusicLocator::GetMusicSystem().Play(static_cast<MusicId>(DiggerUtils::MusicDiggerID::WIN), false);
}

void PlayState::SetPlayerWon()
{
	m_PlayerHasWon = true;
}

void PlayState::Exit(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("isPlayerDead", false);
	pBlackboard->ChangeValue("hasPlayerWon", false);
	pBlackboard->ChangeValue("hasExtraLife", true);
	m_PlayerHasWon = false;
	m_PlayerIsDead = false;
	EventManager::GetInstance().RemoveEvent("PlayerDied", this, &PlayState::HandlePlayerDead);
	EventManager::GetInstance().RemoveEvent("PlayerWon", this, &PlayState::HandlePlayerWon);
}

void RespawnState::Enter(Blackboard* pBlackboard)
{
	EventManager::GetInstance().AddEvent("LevelLoaded", this, &RespawnState::HasLoadedLevel);
	pBlackboard->ChangeValue("hasLoadedLevel", false);
	EventManager::GetInstance().NotifyEvent("ReloadCurrentLevel");
}

void RespawnState::Exit(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("hasLoadedLevel", false);
}

void RespawnState::Update(Blackboard* pBlackboard)
{
	if (m_HasLoaded)
	{
		pBlackboard->ChangeValue("hasLoadedLevel", true);
	}
}

void RespawnState::HasLoadedLevel()
{
	m_HasLoaded = true;
}

void GameOverState::Enter(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("hasSetName", false);
	// Show the Widget GameOver and High-Score
	WidgetManager::GetInstance().SetActiveWidget("GameOver");
	EventManager::GetInstance().AddEvent("SetNameHighScore", this, &GameOverState::HasSetName);
}

void GameOverState::Exit(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("hasSetName", false);
	// Remove the widget GameOver and High-Score
	WidgetManager::GetInstance().SetActiveWidget("MainMenu");
	// Go back to the menu
}

void GameOverState::Update(Blackboard* pBlackboard)
{
	if (m_HasSetName)
	{
		pBlackboard->ChangeValue("hasSetName", true);
	}
}

void GameOverState::HasSetName()
{
	m_HasSetName = true;
}

void IdleStateMoneyBag::Enter(Blackboard* pBlackboard)
{
	pBlackboard->GetValue("MoneyBagStates", m_State);
	switch (m_State)
	{
	case MoneyBagComponent::StateMoneyBag::IsDestroyed:
	case MoneyBagComponent::StateMoneyBag::IdleDestroyed:
	case MoneyBagComponent::StateMoneyBag::Idle:

		break;
	case MoneyBagComponent::StateMoneyBag::CanFall:
	case MoneyBagComponent::StateMoneyBag::IsFalling:
		std::cout << "IdleStateMoneyBag::Enter: Something went wrong\n";
		break;
	}
	pBlackboard->GetValue("Position", m_Position);
}

void IdleStateMoneyBag::Exit(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("MoneyBagStates", m_State);
}

void IdleStateMoneyBag::Update(Blackboard* pBlackboard)
{
	// TODO check if the bottom node is empty
	const auto goLevel{dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>()};
	const auto node{goLevel->GetComponent<LevelComponent>()->GetGraph()->GetClosestNode(m_Position)};
	if (node->GetBottomNeighbor()->CanBeVisited() && m_State == MoneyBagComponent::StateMoneyBag::Idle)
	{
		m_State = MoneyBagComponent::StateMoneyBag::CanFall;
		pBlackboard->ChangeValue("MoneyBagStates", m_State);
		TimerManager::GetInstance().AddTimer(this, &IdleStateMoneyBag::SetFallingState, 1.5f);
	}
	if (m_State == MoneyBagComponent::StateMoneyBag::IsFalling)
	{
		pBlackboard->ChangeValue("MoneyBagStates", m_State);
	}
}

void IdleStateMoneyBag::SetFallingState()
{
	m_State = MoneyBagComponent::StateMoneyBag::IsFalling;
}

void FallingStateMoneyBag::Enter(Blackboard* pBlackboard)
{
	m_NodeTravelled.clear();
	EventManager::GetInstance().AddEvent("MoneyBagLanded", this, &FallingStateMoneyBag::HandleMoneyBagLanded);
	pBlackboard->GetValue("MoneyBagStates", m_State);
	pBlackboard->GetValue("MoneyBagObject", m_MoneyBagObject);
	if (!m_MoneyBagObject->HasComponent<MoneyBagComponent>())
	{
		std::cerr << "FallingStateMoneyBag::Enter: Object does not have money bag component\n";
	}

	const auto levelObject{dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>()};
	const auto levelComp{levelObject->GetComponent<LevelComponent>()};
	m_NodeTravelled.emplace_back(levelComp->GetGraph()->GetClosestNode(m_MoneyBagObject->GetWorldPosition()));
}

void FallingStateMoneyBag::Exit(Blackboard* pBlackboard)
{
	EventManager::GetInstance().RemoveEvent("MoneyBagLanded", this, &FallingStateMoneyBag::HandleMoneyBagLanded);
	pBlackboard->ChangeValue("MoneyBagStates", m_State);
}

void FallingStateMoneyBag::Update(Blackboard* pBlackboard)
{
	if (m_State != MoneyBagComponent::StateMoneyBag::IsFalling)
	{
		pBlackboard->ChangeValue("MoneyBagStates", m_State);
		return;
	}
	// Fall to the bottom node
	m_MoneyBagObject->Translate(TimeEngine::GetInstance().GetDeltaTime() * m_FallSpeed * m_Direction);

	// Check if the object is in another node
	const auto levelObject{dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>()};
	const auto levelComp{levelObject->GetComponent<LevelComponent>()};
	const auto node{levelComp->GetGraph()->GetClosestNode(m_MoneyBagObject->GetWorldPosition())};
	if (std::ranges::find(m_NodeTravelled, node) == m_NodeTravelled.end())
	{
		m_NodeTravelled.emplace_back(node);
	}
}

void FallingStateMoneyBag::HandleMoneyBagLanded()
{
	if (m_NodeTravelled.size() <= 2)
	{
		m_State = MoneyBagComponent::StateMoneyBag::Idle;
		return;
	}
	m_State = MoneyBagComponent::StateMoneyBag::IsDestroyed;
}
