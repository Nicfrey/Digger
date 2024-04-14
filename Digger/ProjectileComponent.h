#pragma once
#include "BaseComponent.h"
#include "Utils.h"

class ProjectileComponent : public BaseComponent
{
public:
	ProjectileComponent();
	ProjectileComponent(const glm::vec2& direction);
	~ProjectileComponent() override = default;
	ProjectileComponent(const ProjectileComponent& other) = delete;
	ProjectileComponent(ProjectileComponent&& other) noexcept = delete;
	ProjectileComponent& operator=(const ProjectileComponent& other) = delete;
	ProjectileComponent& operator=(ProjectileComponent&& other) noexcept = delete;

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

