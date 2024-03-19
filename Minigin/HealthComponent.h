#pragma once
#include "BaseComponent.h"

class HealthComponent : public BaseComponent
{
public:
	HealthComponent();
	void LoseOneLife();
	void GainOneLife();
	int GetLifeRemaining() const;

	void Update() override;
	void FixedUpdate() override;
	void Init() override;
	void Render() const override {}
	void RenderGUI() override;

	
private:
	int m_LifeRemaining;

};

