#include "NavMeshAgentComponent.h"

#include "GameObject.h"
#include "Graph.h"
#include "SceneManager.h"
#include "TimeEngine.h"
#include "Utils.h"
#include "../Digger/PlayerComponent.h"

NavMeshAgentComponent::NavMeshAgentComponent(GraphUtils::Graph* graph, float speed) : BaseComponent{ nullptr }, m_Target(),
                                                                                      m_HasReachedDestination(false), m_Direction()
{
	m_Graph = graph;
	m_Speed = speed;
}


void NavMeshAgentComponent::SetPath(const glm::vec2& pos)
{
	m_Path = m_Graph->GetShortestPath(m_Graph->GetClosestNode(GetGameObject()->GetWorldPosition()),
	                                  m_Graph->GetClosestNode(glm::vec3{pos, 0}));
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
	const auto pos{GetGameObject()->GetWorldPosition()};

	// Create the sphere of acceptance
	const Circlef sphereOfAcceptance{m_Target, m_AcceptanceRadius};
	if (IsPointInCircle(glm::vec2{pos}, sphereOfAcceptance))
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
