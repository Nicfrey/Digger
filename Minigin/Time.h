#pragma once
#include <chrono>

class Time final
{
public:
	Time();
	Time(uint64_t targetFps);
	void UpdateLastTime();
	void Update();
	static float GetDeltaTime();
	static uint64_t GetLastTime();
	static uint64_t GetCurrent();
	static float GetFrameTime();


private:
	int m_CounterFrame{};
	float m_TimerFps{};
	double m_Fps{};
	uint64_t m_TargetFps;
	static float m_FrameTime;
	static float m_DeltaTime;
	static uint64_t m_LastTime;
	static uint64_t m_CurrentTime;
};

