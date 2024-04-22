#include "Graph.h"

#include <iostream>
#include <glm/geometric.hpp>

int GraphUtils::GraphNode::m_IdCounter = 0;

GraphUtils::GraphNode::GraphNode(bool canBeVisited): m_Id(++m_IdCounter), m_Neighbors{}, m_Position(glm::vec3(0.0f)), m_CanBeVisited{canBeVisited}
{
}

GraphUtils::GraphNode::GraphNode(const glm::vec3& value, bool canBeVisited): GraphNode{canBeVisited}
{
	m_Position = value;
}

GraphUtils::GraphNode::GraphNode(const GraphNode& other)
{
	m_Id = other.m_Id;
	m_Position = other.m_Position;
	m_CanBeVisited = other.m_CanBeVisited;
	for (const auto& neighbor : other.m_Neighbors)
	{
		m_Neighbors.insert(std::make_pair(new GraphNode{ *neighbor.first },neighbor.second));
	}
}

GraphUtils::GraphNode::GraphNode(GraphNode&& other) noexcept
{
	m_Id = other.m_Id;
	m_Position = other.m_Position;
	m_CanBeVisited = other.m_CanBeVisited;
	m_Neighbors = std::move(other.m_Neighbors);
	other.m_Id = 0;
	other.m_Position = glm::vec3(0.0f);
}

GraphUtils::GraphNode& GraphUtils::GraphNode::operator=(const GraphNode& other)
{
	if(this == &other)
	{
		return *this;
	}
	m_Id = other.m_Id;
	m_Position = other.m_Position;
	m_CanBeVisited = other.m_CanBeVisited;
	for (const auto& neighbor : other.m_Neighbors)
	{
		m_Neighbors.insert(std::make_pair(new GraphNode{ *neighbor.first }, neighbor.second));
	}
	return *this;
}

GraphUtils::GraphNode& GraphUtils::GraphNode::operator=(GraphNode&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}
	m_Id = other.m_Id;
	m_Position = other.m_Position;
	m_CanBeVisited = other.m_CanBeVisited;
	m_Neighbors = std::move(other.m_Neighbors);
	other.m_Id = 0;
	other.m_Position = glm::vec3(0.0f);
	return *this;
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

GraphUtils::Graph::Graph(const Graph& other)
{
	for(const auto node : other.m_Nodes)
	{
		m_Nodes.push_back(new GraphNode(*node));
	}
}

GraphUtils::Graph::Graph(Graph&& other) noexcept
{
	m_Nodes = std::move(other.m_Nodes);
	other.m_Nodes.clear();
}

GraphUtils::Graph& GraphUtils::Graph::operator=(const Graph& other)
{
	if(this == &other)
	{
		return *this;
	}
	for (auto node : m_Nodes)
	{
		delete node;
		node = nullptr;
	}
	m_Nodes.clear();
	for (const auto node : other.m_Nodes)
	{
		m_Nodes.push_back(new GraphNode(*node));
	}
	return *this;
}

GraphUtils::Graph& GraphUtils::Graph::operator=(Graph&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}
	for (auto node : m_Nodes)
	{
		delete node;
		node = nullptr;
	}
	m_Nodes.clear();
	m_Nodes = std::move(other.m_Nodes);
	other.m_Nodes.clear();
	return *this;
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

GraphUtils::GraphNode* GraphUtils::Graph::GetClosestNode(const glm::vec3& position) const
{
	GraphNode* closestNode = nullptr;
	float minDistance = FLT_MAX;
	for (const auto node : m_Nodes)
	{
		const float distance = glm::distance(node->GetPosition(), position);
		if(distance < minDistance)
		{
			minDistance = distance;
			closestNode = node;
		}
	}
	return closestNode;
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
	float distance{ GetDistance(pStart,pEnd) };
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
		pCurrent = GetNodeMinDistance(pEnd, openList,closedList,distance);
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


GraphUtils::GraphNode* GraphUtils::Graph::GetNodeMinDistance(const GraphUtils::GraphNode* endNode, const std::list<GraphUtils::GraphNode*>& openList, const std::list<GraphUtils::
                                                             GraphNode*>& closedList, float& distance)
{
	std::cout << "distance: " << distance << '\n';
	GraphNode* minNode{nullptr};
	float minDistance {FLT_MAX};
	for (const auto neighbor : openList)
	{
		auto it = std::ranges::find(closedList, neighbor);
		if(it != closedList.end())
		{
			continue;
		}
		const float distanceNeighborEnd = GetDistance(neighbor, endNode);
		if(distanceNeighborEnd < minDistance && neighbor->CanBeVisited())
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
