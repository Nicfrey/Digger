#include "Achievement.h"

#include <isteamuserstats.h>
#include <isteamutils.h>
#include <SDL_syswm.h>
#include <cstdio>
#include <isteamuser.h>

#include "Observer.h"


Achievement::Achievement(Achievement_t* Achievements, int NumAchievements) :
	m_CallbackUserStatsReceived(this, &Achievement::OnUserStatsReceived),
	m_CallbackUserStatsStored(this, &Achievement::OnUserStatsStored),
	m_CallbackAchievementStored(this, &Achievement::OnAchievementStored),
	m_iAppID(0),
	m_bInitialized(false)
{
	m_iAppID = SteamUtils()->GetAppID();
	m_pAchievements = Achievements;
	m_iNumAchievements = NumAchievements;
	RequestStats();

	EventManager::GetInstance().AddEvent("WinTheGame", [this]() { WinAchievement(); });
}


void Achievement::WinAchievement()
{
	SetAchievement("ACH_WIN_ONE_GAME");
	EventManager::GetInstance().RemoveEvent("WinTheGame", [this]() { WinAchievement(); });
}

bool Achievement::RequestStats()
{
	// Is Steam loaded? If not we can't get stats.
	if (NULL == SteamUserStats() || NULL == SteamUser())
	{
		return false;
	}
	// Is the user logged on?  If not we can't get stats.
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}
	// Request user stats.
	return SteamUserStats()->RequestCurrentStats();
}

bool Achievement::SetAchievement(const char* ID)
{
	// Have we received a call back from Steam yet?
	if (m_bInitialized)
	{
		if(!IsAchievementUnlocked(ID))
		{
			SteamUserStats()->SetAchievement(ID);
			return SteamUserStats()->StoreStats();
		}
	}
	// If not then we can't set achievements yet
	return false;
}

void Achievement::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (static_cast<uint64>(m_iAppID) == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			OutputDebugString(L"Received stats and achievements from Steam\n");
			m_bInitialized = true;

			// load achievements
			for (int iAch = 0; iAch < m_iNumAchievements; ++iAch)
			{
				Achievement_t& ach = m_pAchievements[iAch];

				SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);
				_snprintf_s(ach.m_rgchName, sizeof(ach.m_rgchName), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"name"));
				_snprintf_s(ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"desc"));
			}
		}
		else
		{
			char buffer[128];
			_snprintf_s(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
			OutputDebugString(reinterpret_cast<LPCWSTR>(buffer));
		}
	}
}

void Achievement::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (static_cast<uint64>(m_iAppID) == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			OutputDebugString(L"Stored stats for Steam\n");
		}
		else
		{
			char buffer[128];
			_snprintf_s(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
			OutputDebugString(reinterpret_cast<LPCWSTR>(buffer));
		}
	}
}

void Achievement::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (static_cast<uint64>(m_iAppID) == pCallback->m_nGameID)
	{
		OutputDebugString(L"Stored Achievement for Steam\n");
	}
}

bool Achievement::IsAchievementUnlocked(const char* ID)
{
	bool unlocked{ false };
	if(SteamUserStats()->GetAchievement(ID,&unlocked))
	{
		return unlocked;
	}
	return false;
}
