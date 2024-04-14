#pragma once
#include <glm/vec3.hpp>

#include "BaseComponent.h"

class RotateComponent final : public BaseComponent
{
public:
	RotateComponent(const glm::vec3& pointToRotate = {}, float speed = 5.f, float startAngle = 0.f);
	void Update() override;
	void FixedUpdate() override;
	void Init() override;
	void Render() const override;
	void RenderGUI() override;

private:
	glm::vec3 m_PointToRotate;
	float m_CurrentAngle;
	float m_StartAngle;
	float m_Speed;
	glm::vec3 m_StartPoint;
};

