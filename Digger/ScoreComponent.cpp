#include "ScoreComponent.h"

#include "GameInstance.h"
#include "Observer.h"

std::shared_ptr<BaseComponent> ScoreComponent::Clone() const
{
	return std::make_shared<ScoreComponent>(*this);
}

void ScoreComponent::AddScore(int score)
{
	m_Score += score;
	int totalScore;
	GameInstance::GetInstance().GetValue("Score", totalScore);
	totalScore += score;
	if(totalScore / m_ScoreLifeUp != m_CanGainLife)
	{
		m_CanGainLife = totalScore / m_ScoreLifeUp;
		EventManager::GetInstance().NotifyEvent("GainLife");
	}
	GameInstance::GetInstance().ChangeValue("Score", totalScore);
	EventManager::GetInstance().NotifyEvent("ScoreAdded");
}

int ScoreComponent::GetScore() const
{
	return m_Score;
}
