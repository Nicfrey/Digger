#pragma once
#include "SoundSystemEngine.h"
#include "Utils.h"

namespace DiggerUtils
{
#define NUMBER_LEVEL_AVAILABLE 3

	enum class SoundDiggerID : SoundId
	{
		PROJECTILE_SHOOT,
		PROJECTILE_HIT,
		ENEMY_DIED,
		PLAYER_DIED
	};

	enum class MusicDiggerID : MusicId
	{
		MAIN_MENU,
		GAME,
		PLAYER_DIED,
		WIN,
		GAME_OVER
	};

	enum class DiggerGameMode
	{
		SinglePlayer,
		Coop,
		Versus
	};

	void OnLoadLevel1();
	void OnLoadLevel2();
	void OnLoadLevel3();
	void LoadGameOver();
	void NextLevel();

	void SelectSinglePlayer();
	void SelectCoop();
	void SelectVersus();

	void HasSetName();

	void ReturnToMenu();
}
