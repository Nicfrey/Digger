#pragma once
#include "TextComponent.h"


class FpsComponent;

class FpsTextComponent : public dae::TextComponent
{
public:
	FpsTextComponent(const std::shared_ptr<dae::Font>& font);
	void Init() override;
	void Update() override;
	void Render() const override;
	void SetPrecision(int precision);
	void FixedUpdate() override;

private:
	int m_Precision{1};
	std::shared_ptr<FpsComponent> m_FpsComponent;
};

