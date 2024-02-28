#include "Transform.h"

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::Transform::SetPosition(float x, float y)
{
	SetPosition(x, y, 0.f);
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
