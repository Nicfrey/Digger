#pragma once
#include "BaseComponent.h"

class HealthComponent : public BaseComponent
{
public:
	HealthComponent();
	HealthComponent(int lifeRemaining);
	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
	void LoseOneLife();
	int GetLifeRemaining() const;
	bool HasNoRemainingLife() const;
	bool IsDead() const;
	void OnDestroy() override;
	void SetAlive();
;private:
	void GainOneLife();
	int m_LifeRemaining;
	bool m_Alive{ true };
};

