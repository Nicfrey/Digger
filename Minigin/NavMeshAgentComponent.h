#pragma once
#include <vector>
#include <glm/vec2.hpp>

#include "BaseComponent.h"


namespace GraphUtils
{
	class GraphNode;
}

namespace GraphUtils
{
	class Graph;
}

class NavMeshAgentComponent : public BaseComponent
{
public:
	NavMeshAgentComponent(GraphUtils::Graph* graph, float speed = 100.f, bool canAvoidObstacle = false);
	~NavMeshAgentComponent() override = default;
	void SetPath(const glm::vec2& pos);
	void Update() override;
	std::shared_ptr<BaseComponent> Clone() const override;
	bool HasReachedDestination() const { return m_HasReachedDestination; }
	void SetSpeed(float speed);
	void StopMoving();

private:
	std::vector<GraphUtils::GraphNode*> m_Path;
	glm::vec2 m_Target;
	float m_Speed;
	bool m_HasReachedDestination;
	float m_AcceptanceRadius{ 5.f };
	GraphUtils::Graph* m_Graph;
	glm::vec2 m_Direction;
	bool m_CanAvoidObstacle;
	bool IsNearTheTarget() const;
};

