#include "DiggerStates.h"

#include "Blackboard.h"
#include "DiggerUtils.h"
#include "Observer.h"
#include "SoundSystemEngine.h"
#include "Utils.h"
#include "WidgetManager.h"

void MenuState::Enter(Blackboard* pBlackboard)
{
	// TODO Play menu music
	// Add function to event
	EventManager::GetInstance().AddEvent("SelectGameMode",this,&MenuState::HasSelectedGameMode);
	EventManager::GetInstance().AddEvent("CancelGameMode", this, &MenuState::CancelGameMode);
	EventManager::GetInstance().AddEvent("SelectLevel", this, &MenuState::HasSelectedLevel);
	// Change the state of load level
	pBlackboard->ChangeValue("hasSelectedLevel", false);
}

void MenuState::Update(Blackboard* pBlackboard)
{
	// TODO Check if player has selected a game mode and a level
	if(m_HasSelectedGameMode && m_HasSelectedLevel)
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
	if(m_HasLoadedLevel)
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
	pBlackboard->ChangeValue("isPlayerDead", false);
	pBlackboard->ChangeValue("hasPlayerWon", false);
	EventManager::GetInstance().AddEvent("PlayerDied",this,&PlayState::HandlePlayerDead);
	EventManager::GetInstance().AddEvent("PlayerWon", this, &PlayState::HandlePlayerWon);
	// TODO Play music of the game
	ServiceMusicLocator::GetMusicSystem().Play(static_cast<MusicId>(DiggerUtils::MusicDiggerID::GAME), true);
}

void PlayState::Update(Blackboard* pBlackboard)
{
	if(m_PlayerIsDead)
	{
		pBlackboard->ChangeValue("isPlayerDead", true);
		return;
	}
	if(m_PlayerHasWon)
	{
		pBlackboard->ChangeValue("hasPlayerWon",true);
	}
}

void PlayState::HandlePlayerDead()
{
	// Wait 5 sec before setting the player to dead
	TimerManager::GetInstance().AddTimer(this, &PlayState::SetPlayerIsDead, 5.f);
}

void PlayState::SetPlayerIsDead()
{
	m_PlayerIsDead = true;
}

void PlayState::HandlePlayerWon()
{
	// Wait 5 sec before setting the player to won
	TimerManager::GetInstance().AddTimer(this, &PlayState::SetPlayerWon, 5.f);
}

void PlayState::SetPlayerWon()
{
	m_PlayerHasWon = true;
}

void PlayState::Exit(Blackboard* pBlackboard)
{
	pBlackboard->ChangeValue("isPlayerDead", false);
	pBlackboard->ChangeValue("hasPlayerWon", false);
	EventManager::GetInstance().RemoveEvent("PlayerDied", this, &PlayState::HandlePlayerDead);
	EventManager::GetInstance().RemoveEvent("PlayerWon", this, &PlayState::HandlePlayerWon);
}
