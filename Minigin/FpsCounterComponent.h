#pragma once
#include "TextManager.h"

class FpsCounterComponent : public dae::TextManager
{
public:
	FpsCounterComponent(const std::shared_ptr<dae::Font>& font);
	void Init() override;
	void Update() override;
	void Render() const override;
	void SetPrecision(int precision);
private:
	int m_Precision{1};
};

