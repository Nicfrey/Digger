#pragma once
#include "BaseComponent.h"

class HealthComponent : public BaseComponent
{
public:
	HealthComponent();
	void LoseOneLife();
	void GainOneLife();
	int GetLifeRemaining() const;
	bool IsDead() const;
private:
	int m_LifeRemaining;

};

