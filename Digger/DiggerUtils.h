#pragma once
#include "SoundSystemEngine.h"

namespace DiggerUtils
{
	enum class SoundDiggerID : SoundId
	{
		PROJECTILE_SHOOT,
		PROJECTILE_HIT,
		ENEMY_DIED,
		PLAYER_DIED
	};

	void OnLoadLevel1();
	void OnLoadLevel2();
	void OnLoadLevel3();

}
