#pragma once
#include "BaseComponent.h"

class PlayerComponent : public BaseComponent
{
public:
	PlayerComponent();
	~PlayerComponent() override = default;
	PlayerComponent(const PlayerComponent& other);
	PlayerComponent(PlayerComponent&& other) noexcept;
	PlayerComponent& operator=(const PlayerComponent& other);
	PlayerComponent& operator=(PlayerComponent&& other) noexcept;
	std::shared_ptr<BaseComponent> Clone() const override;
	void Update() override;
	void Init() override;
	void RenderGUI() override;
	void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) override;
	void ShootProjectile();
	void ResetProjectile();

private:
	dae::GameObject* m_pProjectile;
	std::unique_ptr<dae::GameObject> m_pDefaultProjectile;
	void HandleCollisionProjectile(std::shared_ptr<dae::GameObject>& other) const;
	std::shared_ptr<dae::GameObject> CreateProjectile() const;
	void SetDeadAnim();
};

