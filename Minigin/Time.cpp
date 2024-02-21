#include "Time.h"

#include <iostream>
#include <SDL_timer.h>

std::chrono::steady_clock::time_point Time::m_CurrentTime = {};
std::chrono::steady_clock::time_point Time::m_LastTime = {};
float Time::m_DeltaTime = 0.f;

void Time::Update()
{
	const auto currentTime{ std::chrono::high_resolution_clock::now() };
	m_CurrentTime = currentTime;

	m_DeltaTime = std::chrono::duration<float>(m_CurrentTime - m_LastTime).count();

	m_LastTime = m_CurrentTime;
}


void Time::UpdateLastTime()
{
	m_LastTime = std::chrono::high_resolution_clock::now();
}

float Time::GetDeltaTime()
{
	return m_DeltaTime;
}

std::chrono::steady_clock::time_point Time::GetLastTime()
{
	return m_LastTime;
}

std::chrono::steady_clock::time_point Time::GetCurrent()
{
	return m_CurrentTime;
}


