#include "MovementComponent.h"

#include "GameObject.h"
#include "imgui.h"
#include "Time.h"

MovementComponent::MovementComponent(float speed): BaseComponent(nullptr), m_IsMoving{}, m_Speed{speed}, m_DesiredVelocity{}, m_DesiredVelocityDebug{m_DesiredVelocity}
{
}

void MovementComponent::Update()
{
	if(m_IsMoving)
	{
		GetGameObject()->SetLocalPosition(GetGameObject()->GetLocalPosition() + m_DesiredVelocity);
		m_IsMoving = false;
		m_DesiredVelocity = glm::vec3{};
	}
}

void MovementComponent::FixedUpdate()
{
}

void MovementComponent::Init()
{
}

void MovementComponent::Render() const
{
}

void MovementComponent::RenderGUI()
{
	bool windowActive{ true };
	ImGui::Begin("MovementComponent",&windowActive);
	ImGui::Text("Desired Velocity: (%f,%f,%f)", m_DesiredVelocityDebug.x, m_DesiredVelocityDebug.y, m_DesiredVelocityDebug.z);
	ImGui::Indent();
	ImGui::End();
}

void MovementComponent::Move(const Direction& direction)
{
	switch (direction)
	{
	case Up:
		m_DesiredVelocity += glm::vec3{ 0,-m_Speed * Time::GetDeltaTime(),0 };
		m_IsMoving = true;
		break;
	case Down:
		m_DesiredVelocity += glm::vec3{ 0,m_Speed * Time::GetDeltaTime(),0 };
		m_IsMoving = true;
		break;
	case Left:
		m_DesiredVelocity += glm::vec3{ -m_Speed * Time::GetDeltaTime(),0,0 };
		m_IsMoving = true;
		break;
	case Right:
		m_DesiredVelocity += glm::vec3{ m_Speed * Time::GetDeltaTime(),0,0 };
		m_IsMoving = true;
		break;
	}
	m_DesiredVelocityDebug = m_DesiredVelocity;
}
