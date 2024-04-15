#pragma once
#include "BaseComponent.h"
#include "Utils.h"

class ProjectileComponent : public BaseComponent
{
public:
	ProjectileComponent();
	ProjectileComponent(const glm::vec2& direction);
	~ProjectileComponent() override = default;
	ProjectileComponent(const ProjectileComponent& other) = default;
	ProjectileComponent(ProjectileComponent&& other) noexcept;
	ProjectileComponent& operator=(const ProjectileComponent& other);
	ProjectileComponent& operator=(ProjectileComponent&& other) noexcept;

	std::shared_ptr<BaseComponent> Clone() const override;
	void Update() override;
	void Init() override;

	void Activate(dae::GameObject* owner);
	void SetDirection(const glm::vec2& direction) { m_Direction = direction; }
	dae::GameObject* GetShotBy() const { return m_ShotBy; }

private:
	bool m_IsActive;
	float m_RotationSpeed;
	glm::vec2 m_Velocity;
	glm::vec2 m_Direction;
	dae::GameObject* m_ShotBy;
};

