#pragma once
#include "ItemComponent.h"

class ScoreComponent;

class EmeraldComponent : public ItemComponent
{
public:
	EmeraldComponent();
	std::shared_ptr<BaseComponent> Clone() const override;
	void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) override;
private:
	static void IncrementInARow();
	static void ResetInARow();
	void AddScoreInARow(const std::shared_ptr<ScoreComponent>& score);
	int m_ScoreValue { 25 };
	int m_ScoreInARow{ 250 };
	static int m_CollectedInARow;
	static constexpr int m_MaxCollectedInARow{ 8 };
};

