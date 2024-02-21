#pragma once
#include <chrono>

class Time final
{
public:
	void UpdateLastTime();
	void Update();
	static float GetDeltaTime();
	static std::chrono::steady_clock::time_point GetLastTime();
	static std::chrono::steady_clock::time_point GetCurrent();
private:
	static float m_DeltaTime;
	static std::chrono::steady_clock::time_point m_LastTime;
	static std::chrono::steady_clock::time_point m_CurrentTime;
};

