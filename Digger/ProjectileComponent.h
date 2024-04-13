#pragma once
#include "BaseComponent.h"
#include "Utils.h"

class ProjectileComponent : public BaseComponent
{
public:
	ProjectileComponent();
	~ProjectileComponent() override = default;
	ProjectileComponent(const ProjectileComponent& other) = delete;
	ProjectileComponent(ProjectileComponent&& other) noexcept = delete;
	ProjectileComponent& operator=(const ProjectileComponent& other) = delete;
	ProjectileComponent& operator=(ProjectileComponent&& other) noexcept = delete;

	void Update() override;
	void Init() override;

	void Activate();

private:
	bool m_IsActive;
	float m_RotationSpeed;
	glm::vec2 m_Velocity;
};

