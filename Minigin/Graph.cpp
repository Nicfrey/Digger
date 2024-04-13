#include "Graph.h"

#include <glm/geometric.hpp>

int GraphUtils::GraphNode::m_IdCounter = 0;

GraphUtils::GraphNode::GraphNode(): m_Id(++m_IdCounter), m_Neighbors{}, m_Position(glm::vec3(0.0f))
{
}

GraphUtils::GraphNode::GraphNode(const glm::vec3& value): GraphNode{}
{
	m_Position = value;
}

GraphUtils::GraphNode::~GraphNode()
{

}

float GraphUtils::GraphNode::GetDistance(const GraphNode* neighbor) const
{
	return distance(m_Position, neighbor->m_Position);
}

void GraphUtils::GraphNode::AddNeighbor(GraphNode* neighbor, float distance)
{
	m_Neighbors.insert(std::make_pair(neighbor, distance));
}

GraphUtils::Graph::~Graph()
{
	for (auto node : m_Nodes)
	{
		delete node;
		node = nullptr;
	}
}

GraphUtils::GraphNode* GraphUtils::Graph::AddNode()
{
	GraphNode* node = new GraphNode{};
	m_Nodes.push_back(node);
	return node;
}

GraphUtils::GraphNode* GraphUtils::Graph::AddNode(const glm::vec3& value)
{
	GraphNode* node = new GraphNode{value};
	m_Nodes.push_back(node);
	return node;
}

GraphUtils::GraphNode* GraphUtils::Graph::GetNode(int index) const
{
	if(index < 0 || index >= static_cast<int>(m_Nodes.size()))
	{
		return nullptr;
	}
	return m_Nodes[index];
}

GraphUtils::GraphNode* GraphUtils::Graph::GetNode(GraphNode* node)
{
	const auto it = std::ranges::find(m_Nodes, node);
	return *it;
}

void GraphUtils::Graph::RemoveNode(GraphNode* node)
{
	const auto it = std::find(m_Nodes.begin(), m_Nodes.end(), node);
	if (it != m_Nodes.end())
	{
		delete* it;
		*it = nullptr;
		m_Nodes.erase(it);
	}
}

std::vector<GraphUtils::GraphNode*> GraphUtils::Graph::GetNodes() const
{
	return m_Nodes;
}

std::vector<GraphUtils::GraphNode*> GraphUtils::Graph::GetShortestPath(GraphNode* pStart, GraphNode* pEnd)
{
	GraphNode* pCurrent = pStart;
	std::vector<GraphNode*> path{};
	std::list<GraphNode*> openList{};
	std::list<GraphNode*> closedList{};
	openList.emplace_back(pCurrent);
	while (!openList.empty())
	{
		for (auto neighbor : pCurrent->GetNeighbors())
		{
			auto it = std::ranges::find_if(openList, [neighbor](const GraphNode* other)
			{
				return other->GetId() == neighbor.first->GetId();
			});
			if(it == openList.end())
			{
				openList.emplace_back(neighbor.first);
			}
		}
		closedList.emplace_back(pCurrent);
		openList.remove(pCurrent);
		pCurrent = GetNodeMinDistance(pEnd, openList);
		if(pCurrent == pEnd)
		{
			break;
		}
	}

	while(pStart != pCurrent)
	{
		path.push_back(pCurrent);
		closedList.remove(pCurrent);
		pCurrent = closedList.back();
	}
	path.push_back(pStart);
	std::reverse(path.begin(), path.end());
	return path;
}


GraphUtils::GraphNode* GraphUtils::Graph::GetNodeMinDistance(const GraphNode* endNode, const std::list<GraphUtils::GraphNode*>& openList)
{
	GraphNode* minNode{nullptr};
	float minDistance {FLT_MAX};
	for (const auto neighbor : openList)
	{
		const float distanceNeighborEnd = GetDistance(neighbor, endNode);
		if(distanceNeighborEnd < minDistance)
		{
			minNode = neighbor;
			minDistance = distanceNeighborEnd;
		}
	}
	return minNode;
}

float GraphUtils::Graph::GetDistance(const GraphNode* start,const GraphNode* end)
{
	return start->GetDistance(end);
}
