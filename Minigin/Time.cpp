#include "Time.h"

#include <iostream>
#include <SDL_timer.h>

uint64_t Time::m_CurrentTime = {};
uint64_t Time::m_LastTime = {};
float Time::m_DeltaTime = 0.f;
float Time::m_FrameTime = {};
double Time::m_Fps = 0.0;

Time::Time(): Time{SDL_GetPerformanceFrequency()}
{
}

Time::Time(uint64_t targetFps): m_TargetFps{targetFps}
{
	m_FrameTime = 1.f / static_cast<float>(targetFps);
}

void Time::Update()
{
	const uint64_t currentTime{ SDL_GetPerformanceCounter() };
	m_CurrentTime = currentTime;

	m_DeltaTime = static_cast<float>((m_CurrentTime - m_LastTime)) * m_FrameTime;

	m_LastTime = m_CurrentTime;
	m_CounterFrame++;

	m_TimerFps += m_DeltaTime;
	if (m_TimerFps >= 1.f)
	{
		m_Fps = m_CounterFrame / m_TimerFps;
		m_TimerFps = 0.f;
		m_CounterFrame = 0;
	}
}

double Time::GetFps()
{
	return m_Fps;
}

void Time::UpdateLastTime()
{
	m_LastTime = SDL_GetPerformanceCounter();
}

float Time::GetDeltaTime()
{
	return m_DeltaTime;
}

uint64_t Time::GetLastTime()
{
	return m_LastTime;
}

uint64_t Time::GetCurrent()
{
	return m_CurrentTime;
}


float Time::GetFrameTime()
{
	return m_FrameTime;
}

