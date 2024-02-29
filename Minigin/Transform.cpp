#include "Transform.h"


dae::Transform::Transform(const glm::vec3& pos) : m_Position{ pos }, m_Rotation{}, m_Scale{1.f}
{
}

void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::Transform::SetLocalPosition(float x, float y)
{
	SetLocalPosition(x, y, 0.f);
}

void dae::Transform::SetLocalPosition(const glm::vec3& pos)
{
	SetLocalPosition(pos.x, pos.y, pos.z);
}

void dae::Transform::SetLocalPosition(const glm::vec2& pos)
{
	SetLocalPosition(pos.x, pos.y);
}

void dae::Transform::SetRotation(float x, float y, float z)
{
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;
}

void dae::Transform::SetRotation(float x, float y)
{
	SetRotation(x, y, 1.f);
}

void dae::Transform::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void dae::Transform::SetScale(float scale)
{
	SetScale(scale, scale, scale);
}
