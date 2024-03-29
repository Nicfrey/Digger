#pragma once
#include "BaseComponent.h"

class ScoreComponent : public BaseComponent
{
public:
	ScoreComponent() = default;
	void AddScore(int score);
	int GetScore() const;
	void Update() override;
	void FixedUpdate() override;
	void Init() override;
	void Render() const override;
	void RenderGUI() override;
private:
	int m_Score;
};

