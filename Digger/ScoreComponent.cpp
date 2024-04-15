#include "ScoreComponent.h"

#include "Observer.h"

std::shared_ptr<BaseComponent> ScoreComponent::Clone() const
{
	return std::make_shared<ScoreComponent>(*this);
}

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
