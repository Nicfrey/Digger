#pragma once
#include "SoundSystemEngine.h"
namespace DiggerUtils
{
#define NUMBER_LEVEL_AVAILABLE 3

	enum class SoundDiggerID : SoundId
	{
		PROJECTILE_SHOOT,
		PROJECTILE_HIT_ENTITIES,
		PROJECTILE_HIT_WALL,
		ENEMY_DIED,
		PLAYER_DIED,
		COLLECT_EMERALD,
		COLLECT_MONEY_BAG,
	};
#define TO_SOUND_ID(soundDiggerID) static_cast<SoundId>(soundDiggerID)

	enum class MusicDiggerID : MusicId
	{
		MAIN_MENU,
		GAME,
		PLAYER_DIED,
		WIN,
		GAME_OVER
	};

#define TO_MUSIC_ID(musicDiggerID) static_cast<MusicId>(musicDiggerID)

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
