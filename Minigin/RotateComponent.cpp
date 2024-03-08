#include "RotateComponent.h"

#include "GameObject.h"
#include "Time.h"

RotateComponent::RotateComponent(const glm::vec3& pointToRotate,float speed ,float startAngle) : BaseComponent{nullptr},
	m_PointToRotate{pointToRotate}, m_CurrentAngle{0.f}, m_StartAngle{startAngle}, m_Speed{speed}, m_StartPoint{}
{
}

void RotateComponent::Update()
{
	m_CurrentAngle += Time::GetDeltaTime() * m_Speed;
	const float sin{ sinf(m_CurrentAngle) };
	const float cos{ cosf(m_CurrentAngle) };
	const float distance{ length(m_StartPoint - m_PointToRotate) };
	glm::vec3 posOrigin{};
	if(GetGameObject()->GetParent() == nullptr)
	{
		posOrigin = glm::vec3{ m_PointToRotate.x + distance * cos,m_PointToRotate.y + distance * sin, 0.f };
	}
	else
	{
		posOrigin = glm::vec3{ distance * cos,distance * sin, 0.f };
	}
	
	GetGameObject()->SetLocalPosition(posOrigin);
}

void RotateComponent::FixedUpdate()
{
}

void RotateComponent::Init()
{
	m_StartPoint = GetGameObject()->GetWorldPosition();
}

void RotateComponent::Render() const
{
}

void RotateComponent::RenderGUI()
{
}
