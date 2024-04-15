#pragma once
#include "BaseComponent.h"

class HealthComponent : public BaseComponent
{
public:
	HealthComponent();
	HealthComponent(int lifeRemaining);
	std::shared_ptr<BaseComponent> Clone() const override;
	void LoseOneLife();
	void GainOneLife();
	int GetLifeRemaining() const;
	bool IsDead() const;
private:
	int m_LifeRemaining;

};

