#pragma once
#include "BaseComponent.h"

class PlayerComponent : public BaseComponent
{
public:
	PlayerComponent();
	~PlayerComponent() override = default;
	void Update() override;
	void RenderGUI() override;
	void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) override;
	void ShootProjectile();
private:
	dae::GameObject* m_pProjectile;

	void HandleCollisionProjectile(std::shared_ptr<dae::GameObject>& other) const;
};

