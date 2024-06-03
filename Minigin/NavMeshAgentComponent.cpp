#include "NavMeshAgentComponent.h"

#include "GameObject.h"
#include "Graph.h"
#include "SceneManager.h"
#include "TimeEngine.h"
#include "Utils.h"

NavMeshAgentComponent::NavMeshAgentComponent(GraphUtils::Graph* graph, float speed, bool canAvoidObstacle) : BaseComponent{ nullptr }, m_Target(),
                                                                                      m_HasReachedDestination(false), m_Direction(), m_CanAvoidObstacle{canAvoidObstacle}
{
	m_Graph = graph;
	m_Speed = speed;
}


void NavMeshAgentComponent::SetPath(const glm::vec2& pos)
{
	m_HasReachedDestination = false;
	m_Path = m_Graph->GetShortestPath(m_Graph->GetClosestNode(GetGameObject()->GetWorldPosition()),
	                                  m_Graph->GetClosestNode(glm::vec3{pos, 0}),m_CanAvoidObstacle);
	std::reverse(m_Path.begin(), m_Path.end());
	if(m_Path.empty() || m_Path.size() == 1) 
	{
		m_HasReachedDestination = true;
		return;
	}
	m_Path.pop_back();
	m_Target = m_Path.back()->GetPosition();
	m_Direction = glm::normalize(m_Target - glm::vec2{GetGameObject()->GetWorldPosition()});
}

void NavMeshAgentComponent::Update()
{
	if (m_HasReachedDestination)
	{
		return;
	}
	GetGameObject()->Translate(m_Direction * m_Speed * TimeEngine::GetInstance().GetDeltaTime());

	if (IsNearTheTarget())
	{
		m_Path.pop_back();
		if (m_Path.empty())
		{
			m_HasReachedDestination = true;
			return;
		}
		m_Target = m_Path.back()->GetPosition();
		m_Direction = glm::normalize(m_Target - glm::vec2{ GetGameObject()->GetWorldPosition() });
	}
}

std::shared_ptr<BaseComponent> NavMeshAgentComponent::Clone() const
{
	return std::make_shared<NavMeshAgentComponent>(*this);
}

void NavMeshAgentComponent::SetSpeed(float speed)
{
	m_Speed = speed;
}

bool NavMeshAgentComponent::IsNearTheTarget() const
{
	const auto pos{ GetGameObject()->GetWorldPosition() };
	const Circlef sphereOfAcceptance{ m_Target, m_AcceptanceRadius };
	return Utils::IsPointInCircle(glm::vec2{ pos }, sphereOfAcceptance);
}
