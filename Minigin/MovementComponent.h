#pragma once
#include <glm/vec3.hpp>

#include "BaseComponent.h"

enum Direction
{
	Up,
	Down,
	Left,
	Right
};

class MovementComponent : public BaseComponent
{
public:
	MovementComponent(float speed = 100.f);
	void Update() override;
	void FixedUpdate() override;
	void Init() override;
	void Render() const override;
	void RenderGUI() override;

	void Move(const Direction& direction);

private:
	bool m_IsMoving;
	float m_Speed;
	glm::vec3 m_DesiredVelocity;
	glm::vec3 m_DesiredVelocityDebug;
};

