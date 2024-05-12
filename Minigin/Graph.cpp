#include "Graph.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <glm/geometric.hpp>

#include "MathUtils.h"

int GraphUtils::GraphNode::m_IdCounter = 0;

GraphUtils::GraphNode::GraphNode(bool canBeVisited): m_Id(++m_IdCounter), m_Neighbors{}, m_Position(glm::vec3(0.0f)),
                                                     m_CanBeVisited{canBeVisited}
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
		m_Neighbors.insert(std::make_pair(new GraphNode{*neighbor.first}, neighbor.second));
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
	if (this == &other)
	{
		return *this;
	}
	m_Id = other.m_Id;
	m_Position = other.m_Position;
	m_CanBeVisited = other.m_CanBeVisited;
	for (const auto& neighbor : other.m_Neighbors)
	{
		m_Neighbors.insert(std::make_pair(new GraphNode{*neighbor.first}, neighbor.second));
	}
	return *this;
}

GraphUtils::GraphNode& GraphUtils::GraphNode::operator=(GraphNode&& other) noexcept
{
	if (this == &other)
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
	for (const auto node : other.m_Nodes)
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
	if (this == &other)
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
	if (this == &other)
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
	if (index < 0 || index >= static_cast<int>(m_Nodes.size()))
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
		if (distance < minDistance)
		{
			minDistance = distance;
			closestNode = node;
		}
	}
	return closestNode;
}

void GraphUtils::Graph::RemoveNode(GraphNode* node)
{
	const auto it = std::ranges::find(m_Nodes, node);
	if (it != m_Nodes.end())
	{
		delete*it;
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
	std::vector<GraphNode*> path{};
	// Create node record and add to openlist
	NodeRecord currentNodeRecord{ pStart, nullptr, 0.f, GetHeuristic(pStart, pEnd) };
	std::list<NodeRecord> openList{};
	std::list<NodeRecord> closedList{};
	openList.emplace_back(currentNodeRecord);

	while (!openList.empty())
	{
		const auto currentNodeRecordIt{ std::min_element(openList.begin(), openList.end()) };
		currentNodeRecord = *currentNodeRecordIt;
		if (currentNodeRecord.pNode->GetId() == pEnd->GetId())
		{
			break;
		}
		for (auto neighbor : currentNodeRecord.pNode->GetNeighbors())
		{
			if(!neighbor.first->CanBeVisited())
			{
				continue;
			}
			const float costSoFar = currentNodeRecord.costSoFar + neighbor.second;
			const float estimatedTotalCost = costSoFar + GetHeuristic(neighbor.first, pEnd);

			// remove from closedList if costSoFar is less expensive

			closedList.remove_if([neighbor, costSoFar](const NodeRecord& node)
				{
					return node.pNode->GetId() == neighbor.first->GetId() && costSoFar < node.costSoFar;
				});
			// remove from openList if costSoFar is less expensive
			openList.remove_if([neighbor, costSoFar](const NodeRecord& node)
				{
					return node.pNode->GetId() == neighbor.first->GetId() && costSoFar < node.costSoFar;
				});

			// Create NodeRecord and add it to openList
			NodeRecord connectionNodeRecord{ neighbor.first, currentNodeRecord.pNode, costSoFar, estimatedTotalCost };
			openList.emplace_back(connectionNodeRecord);
		}
		openList.remove(currentNodeRecord);
		closedList.push_back(currentNodeRecord);
	}

	// Backtracking
	while (pStart != currentNodeRecord.pNode)
	{
		path.emplace_back(currentNodeRecord.pNode);
		auto itExist{
			std::ranges::find_if(closedList,
			                     [&](const NodeRecord& result)
			                     {
				                     return result.pNode->GetId() == currentNodeRecord.pFromNode->GetId();
			                     })
		};
		if (itExist != std::end(closedList))
		{
			currentNodeRecord = *itExist;
		}
	}
	path.emplace_back(pStart);
	std::ranges::reverse(path); // reverses order of the path
	return path;
}


bool GraphUtils::Graph::NodeRecord::operator==(const NodeRecord& other) const
{
	return pNode == other.pNode;
}

bool GraphUtils::Graph::NodeRecord::operator<(const NodeRecord& other) const
{
	return heuristicCost < other.heuristicCost;
}

GraphUtils::GraphNode* GraphUtils::Graph::GetNodeMinDistance(const GraphUtils::GraphNode* endNode,
                                                             const std::list<GraphUtils::GraphNode*>& openList,
                                                             const std::list<GraphUtils::
	                                                             GraphNode*>& closedList)
{
	GraphNode* minNode{nullptr};
	float minDistance{FLT_MAX};
	for (const auto neighbor : openList)
	{
		auto it = std::ranges::find(closedList, neighbor);
		if (it != closedList.end())
		{
			continue;
		}
		const float distanceNeighborEnd{GetDistance(neighbor, endNode)};
		if (distanceNeighborEnd < minDistance && neighbor->CanBeVisited())
		{
			minNode = neighbor;
			minDistance = distanceNeighborEnd;
		}
	}
	return minNode;
}

float GraphUtils::Graph::GetDistance(const GraphNode* start, const GraphNode* end)
{
	return start->GetDistance(end);
}

float GraphUtils::Graph::GetHeuristic(const GraphNode* start, const GraphNode* end)
{
	const glm::vec3 toVector{ end->GetPosition() - start->GetPosition() };
	return std::max(MathUtils::Abs(toVector.x),MathUtils::Abs(toVector.y));
}
