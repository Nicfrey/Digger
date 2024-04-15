#pragma once
#include "ItemComponent.h"

class EmeraldComponent : public ItemComponent
{
public:
	EmeraldComponent();
	void Init() override;
	std::shared_ptr<BaseComponent> Clone() const override;
	void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) override;
private:
	int m_ScoreValue { 25 };
};

