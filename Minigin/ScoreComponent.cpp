#include "ScoreComponent.h"

#include "Achievement.h"
#include "Observer.h"

void ScoreComponent::AddScore(int score)
{
	m_Score += score;
	EventManager::GetInstance().NotifyEvent("ScoreAdded");
	if(m_Score >= 500)
	{
		EventManager::GetInstance().NotifyEvent("WinTheGame");
	}
}

int ScoreComponent::GetScore() const
{
	return m_Score;
}

void ScoreComponent::Update()
{
}

void ScoreComponent::FixedUpdate()
{
}

void ScoreComponent::Init()
{
}

void ScoreComponent::Render() const
{
}

void ScoreComponent::RenderGUI()
{
}
