#pragma once
#include "BaseComponent.h"

class ScoreComponent : public BaseComponent
{
public:
	ScoreComponent() = default;
	std::shared_ptr<BaseComponent> Clone() const override;
	void AddScore(int score);
	int GetScore() const;
private:
	const int m_ScoreLifeUp{ 25000 };
	int m_Score{ 0 };
	int m_CanGainLife{ 0 };
};

