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
	int m_Score;
};

