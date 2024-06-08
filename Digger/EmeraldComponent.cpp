#include "EmeraldComponent.h"

#include <iostream>

#include "DiggerUtils.h"
#include "GameObject.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "ScoreComponent.h"
#include "SoundSystemEngine.h"
#include "Utils.h"

int EmeraldComponent::m_CollectedInARow{ 0 };

EmeraldComponent::EmeraldComponent(): ItemComponent{ItemType::Emerald}
{
}

std::shared_ptr<BaseComponent> EmeraldComponent::Clone() const
{
	return std::make_shared<EmeraldComponent>(*this);
}

void EmeraldComponent::OnCollisionEnter(std::shared_ptr<dae::GameObject>& other)
{
	if(GetGameObject()->IsDestroyed())
	{
		return;
	}
	if(other->HasComponent<PlayerComponent>() && other->HasComponent<ScoreComponent>())
	{
		const auto score{ other->GetComponent<ScoreComponent>() };
		score->AddScore(m_ScoreValue);
		AddScoreInARow(score);
		IncrementInARow();
		EventManager::GetInstance().NotifyEvent("EmeraldCollected");
		ServiceSoundLocator::GetSoundSystem().Play(TO_SOUND_ID(DiggerUtils::SoundDiggerID::COLLECT_EMERALD), 50);
		GetGameObject()->Destroy();
	}
}

void EmeraldComponent::IncrementInARow()
{
	++m_CollectedInARow;
	TimerManager::GetInstance().RemoveTimer(&EmeraldComponent::ResetInARow, 1.f);
	TimerManager::GetInstance().AddTimer(&EmeraldComponent::ResetInARow, 1.f);
}

void EmeraldComponent::ResetInARow()
{
	std::cout << "Resetting in a row\n";
	m_CollectedInARow = 0;
}

void EmeraldComponent::AddScoreInARow(const std::shared_ptr<ScoreComponent>& score) const
{
	if (m_CollectedInARow >= m_MaxCollectedInARow)
	{
		std::cout << "Adding score in a row\n";
		score->AddScore(m_ScoreInARow);
		m_CollectedInARow = 0;
	}
}
