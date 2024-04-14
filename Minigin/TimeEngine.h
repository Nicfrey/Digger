#pragma once
#include "Singleton.h"

class TimeEngine final : public dae::Singleton<TimeEngine>
{
public:
	TimeEngine() = default;
	void SetDeltaTime(float deltaTime);
	float GetDeltaTime();
private:
	float m_DeltaTime;
};