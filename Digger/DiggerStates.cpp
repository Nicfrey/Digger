#include "DiggerStates.h"

#include <iostream>

#include "Blackboard.h"
#include "BoxCollider2D.h"
#include "DiggerUtils.h"
#include "GameInstance.h"
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
#include "HighScoreComponent.h"
#include "imgui.h"
#include "PlayerComponent.h"

void MenuState::Enter(Blackboard* pBlackboard)
{
	std::cout << "MenuState Enter\n";
	ServiceMusicLocator::GetMusicSystem().Play(TO_MUSIC_ID(DiggerUtils::MusicDiggerID::MAIN_MENU), true);
	// Add function to event
	EventManager::GetInstance().AddEvent("SelectGameMode", this, &MenuState::HasSelectedGameMode);
	EventManager::GetInstance().AddEvent("CancelGameMode", this, &MenuState::CancelGameMode);
	EventManager::GetInstance().AddEvent("SelectLevel", this, &MenuState::HasSelectedLevel);
	// Change the state of load level
	pBlackboard->ChangeValue("hasSelectedLevel", false);
}

void MenuState::Update(Blackboard* pBlackboard)
{
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
	GameInstance::GetInstance().ChangeValue("Lives", 3);
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
	std::cout << "LoadState Enter\n";
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
		int level;
		GameInstance::GetInstance().GetValue("CurrentLevel", level);
		pBlackboard->ChangeValue("levelRemaining", NUMBER_LEVEL_AVAILABLE - level);
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
	std::cout << "PlayState Enter\n";
	m_Players = dae::SceneManager::GetInstance().GetGameObjectsWithComponent<PlayerComponent>();
	pBlackboard->ChangeValue("isPlayerDead", false);
	pBlackboard->ChangeValue("hasPlayerWon", false);
	pBlackboard->ChangeValue("hasNoExtraLife", false);
	pBlackboard->ChangeValue("hasSkippedLevel", false);
	EventManager::GetInstance().AddEvent("PlayerDied", this, &PlayState::HandlePlayerDead);
	EventManager::GetInstance().AddEvent("PlayerWon", this, &PlayState::HandlePlayerWon);
	EventManager::GetInstance().AddEvent("NextLevel", this, &PlayState::SkipLevel);
	WidgetManager::GetInstance().SetActiveWidget("GameUI");
	ServiceMusicLocator::GetMusicSystem().Play(TO_MUSIC_ID(DiggerUtils::MusicDiggerID::GAME), true);
}

void PlayState::Update(Blackboard* pBlackboard)
{
	if (m_PlayerIsDead)
	{
		if (m_Players[0]->GetComponent<HealthComponent>()->HasNoRemainingLife())
		{
			pBlackboard->ChangeValue("hasNoExtraLife", true);
			return;
		}
		pBlackboard->ChangeValue("isPlayerDead", true);
		return;
	}
	if (m_PlayerHasWon)
	{
		DiggerUtils::NextLevel();
		if(HasRemainingLevel(pBlackboard))
		{
			pBlackboard->ChangeValue("hasPlayerWon", true);
		}
	}
	if(m_HasSkippedLevel)
	{
		if(HasRemainingLevel(pBlackboard))
			pBlackboard->ChangeValue("hasSkippedLevel", true);

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
		ServiceMusicLocator::GetMusicSystem().Play(TO_MUSIC_ID(DiggerUtils::MusicDiggerID::PLAYER_DIED), false);
		GameInstance::GetInstance().ChangeValue("Lives", m_Players[0]->GetComponent<HealthComponent>()->GetLifeRemaining());
		EventManager::GetInstance().NotifyEvent("LifeLost");
	}
}

void PlayState::SetPlayerIsDead()
{
	m_PlayerIsDead = true;
}

void PlayState::HandlePlayerWon()
{
	// Wait 5 sec before setting the player to won
	TimerManager::GetInstance().AddTimer(this, &PlayState::SetPlayerWon, 2.f);
	ServiceMusicLocator::GetMusicSystem().Play(TO_MUSIC_ID(DiggerUtils::MusicDiggerID::WIN), false);
}

void PlayState::SetPlayerWon()
{
	m_PlayerHasWon = true;
}

void PlayState::SkipLevel()
{
	m_HasSkippedLevel = true;
}

bool PlayState::HasRemainingLevel(Blackboard* pBlackboard) const
{
	int level;
	GameInstance::GetInstance().GetValue("CurrentLevel", level);
	pBlackboard->ChangeValue("levelRemaining", NUMBER_LEVEL_AVAILABLE - level);
	return NUMBER_LEVEL_AVAILABLE - level >= 0;
}


void PlayState::Exit(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("isPlayerDead", false);
	pBlackboard->ChangeValue("hasPlayerWon", false);
	pBlackboard->ChangeValue("hasNoExtraLife", false);
	pBlackboard->ChangeValue("hasSkippedLevel", false);
	m_PlayerHasWon = false;
	m_PlayerIsDead = false;
	m_HasSkippedLevel = false;
	EventManager::GetInstance().RemoveEvent("PlayerDied", this, &PlayState::HandlePlayerDead);
	EventManager::GetInstance().RemoveEvent("PlayerWon", this, &PlayState::HandlePlayerWon);
	EventManager::GetInstance().RemoveEvent("NextLevel", this, &PlayState::SkipLevel);
	TimerManager::GetInstance().RemoveTimer(this, &PlayState::SetPlayerWon, 2.f);
	TimerManager::GetInstance().RemoveTimer(this, &PlayState::SetPlayerIsDead, 5.f);
}

void RespawnState::Enter(Blackboard* pBlackboard)
{
	std::cout << "RespawnState Enter\n";
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
	std::cout << "GameOverState Enter\n";
	pBlackboard->ChangeValue("hasSetName", false);
	// Show the Widget GameOver and High-Score
	WidgetManager::GetInstance().SetActiveWidget("GameOver");
	EventManager::GetInstance().AddEvent("SetNameHighScore", this, &GameOverState::HasSetName);
	EventManager::GetInstance().NotifyEvent("GameOver");
	ServiceMusicLocator::GetMusicSystem().Play(TO_MUSIC_ID(DiggerUtils::MusicDiggerID::GAME_OVER), true);
}

void GameOverState::Exit(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("hasWrittenHighScore", false);
	// Remove the widget GameOver and High-Score
	m_HasSetName = false;
	pBlackboard->ChangeValue("hasWrittenHighScore", m_HasSetName);
}

void GameOverState::Update(Blackboard* pBlackboard)
{
	if (m_HasSetName)
	{
		pBlackboard->ChangeValue("hasWrittenHighScore", true);
	}
}

void GameOverState::HasSetName()
{
	m_HasSetName = true;
}

void ScoreState::Enter(Blackboard* pBlackboard)
{
	std::cout << "ScoreState Enter\n";
	m_ReturnToMainMenu = false;
	EventManager::GetInstance().AddEvent("ReturnToMenu",this,&ScoreState::ReturnToMainMenu);
	pBlackboard->ChangeValue("ReturnToMainMenu", m_ReturnToMainMenu);
	WidgetManager::GetInstance().SetActiveWidget("DisplayScore");
	auto highScoreComp = dae::SceneManager::GetInstance().GetGameObjectWithComponent<HighScoreComponent>()->GetComponent<HighScoreComponent>();
	highScoreComp->DisplayTheScore(true);
	highScoreComp->Init();
}

void ScoreState::Exit(Blackboard* pBlackboard)
{
	m_ReturnToMainMenu = false;
	pBlackboard->ChangeValue("ReturnToMainMenu", m_ReturnToMainMenu);
	dae::SceneManager::GetInstance().SetActiveScene("MainMenu");
	WidgetManager::GetInstance().SetActiveWidget("MainMenu");
	EventManager::GetInstance().RemoveEvent("ReturnToMenu", this, &ScoreState::ReturnToMainMenu);
}

void ScoreState::Update(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("ReturnToMainMenu", m_ReturnToMainMenu);
}

void ScoreState::ReturnToMainMenu()
{
	m_ReturnToMainMenu = true;
}

void IdleStateMoneyBag::Enter(Blackboard* pBlackboard)
{
	pBlackboard->GetValue("MoneyBagObject", m_MoneyBagObject);
	pBlackboard->GetValue("MoneyBagState", m_State);
	switch (m_State)
	{
	case MoneyBagComponent::StateMoneyBag::IsDestroyed:
		TimerManager::GetInstance().AddTimer(this, &IdleStateMoneyBag::SetIdleDestroyed, 0.5f);
		break;
	case MoneyBagComponent::StateMoneyBag::IdleDestroyed:
	case MoneyBagComponent::StateMoneyBag::Idle:
		break;
	case MoneyBagComponent::StateMoneyBag::CanFall:
	case MoneyBagComponent::StateMoneyBag::IsFalling:
		std::cout << "IdleStateMoneyBag::Enter: Something went wrong\n";
		break;
	}
}

void IdleStateMoneyBag::Exit(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("MoneyBagState", m_State);
	TimerManager::GetInstance().RemoveTimer(this, &IdleStateMoneyBag::SetIdleDestroyed, 0.5f);
}

void IdleStateMoneyBag::Update(Blackboard* pBlackboard)
{
	ChangeSizeCollider();
	CheckBottomNode(pBlackboard);
	if (m_State == MoneyBagComponent::StateMoneyBag::IsFalling)
	{
		pBlackboard->ChangeValue("MoneyBagState", m_State);
	}
	if (m_State == MoneyBagComponent::StateMoneyBag::IdleDestroyed)
	{
		pBlackboard->ChangeValue("MoneyBagState", m_State);
	}
}

void IdleStateMoneyBag::SetFallingState()
{
	m_State = MoneyBagComponent::StateMoneyBag::IsFalling;
}

void IdleStateMoneyBag::SetIdleDestroyed()
{
	m_State = MoneyBagComponent::StateMoneyBag::IdleDestroyed;
}

void IdleStateMoneyBag::CheckBottomNode(Blackboard* pBlackboard)
{
	const auto goLevel{ dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>() };
	const auto node{ goLevel->GetComponent<LevelComponent>()->GetGraph()->GetClosestNode(m_MoneyBagObject->GetWorldPosition()) };
	const auto bottomNode{ node->GetBottomNeighbor() };
	if (bottomNode != nullptr && (bottomNode->CanBeVisited() && m_State == MoneyBagComponent::StateMoneyBag::Idle))
	{
		m_State = MoneyBagComponent::StateMoneyBag::CanFall;
		pBlackboard->ChangeValue("MoneyBagState", m_State);
		TimerManager::GetInstance().AddTimer(this, &IdleStateMoneyBag::SetFallingState, 1.5f);
	}
}

void IdleStateMoneyBag::ChangeSizeCollider() const
{
	if (m_State == MoneyBagComponent::StateMoneyBag::IsDestroyed || m_State == MoneyBagComponent::StateMoneyBag::IdleDestroyed)
	{
		m_MoneyBagObject->GetComponent<BoxCollider2D>()->SetSize({ glm::vec2{m_MoneyBagObject->GetWorldPosition().x,m_MoneyBagObject->GetWorldPosition().y - 30},15,30 });
	}
}

void FallingStateMoneyBag::Enter(Blackboard* pBlackboard)
{
	m_NodeTravelled.clear();
	EventManager::GetInstance().AddEvent("MoneyBagLanded", this, &FallingStateMoneyBag::HandleMoneyBagLanded);
	pBlackboard->GetValue("MoneyBagState", m_State);
	pBlackboard->GetValue("MoneyBagObject", m_MoneyBagObject);
	if (!m_MoneyBagObject->HasComponent<MoneyBagComponent>())
	{
		std::cerr << "FallingStateMoneyBag::Enter: Object does not have money bag component\n";
	}

	const auto levelObject{dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>()};
	const auto levelComp{levelObject->GetComponent<LevelComponent>()};
	const auto currentNode{ levelComp->GetGraph()->GetClosestNode(m_MoneyBagObject->GetWorldPosition()) };
	levelComp->FreeSpaceMoneyBag(currentNode);
	m_NodeTravelled.emplace_back(currentNode);
}

void FallingStateMoneyBag::Exit(Blackboard* pBlackboard)
{
	EventManager::GetInstance().RemoveEvent("MoneyBagLanded", this, &FallingStateMoneyBag::HandleMoneyBagLanded);
	pBlackboard->ChangeValue("MoneyBagStates", m_State);
}

void FallingStateMoneyBag::Update(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("MoneyBagState", m_State);
	// Fall to the bottom node
	m_MoneyBagObject->Translate(TimeEngine::GetInstance().GetDeltaTime() * m_FallSpeed * m_Direction);

	CheckBottomNode();
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

void FallingStateMoneyBag::CheckBottomNode()
{
	// Check if the object is in another node
	const auto levelObject{ dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>() };
	const auto levelComp{ levelObject->GetComponent<LevelComponent>() };
	const auto node{ levelComp->GetGraph()->GetClosestNode(m_MoneyBagObject->GetWorldPosition()) };
	if (std::ranges::find(m_NodeTravelled, node) == m_NodeTravelled.end())
	{
		m_NodeTravelled.emplace_back(node);
		// Check if the bottom node can be visited
	}
}
