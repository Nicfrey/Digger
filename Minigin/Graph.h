#pragma once
#include <list>
#include <map>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace GraphUtils
{

	struct GraphTransition
	{
		float cost{};
		bool canVisit{};
	};
	/**
	 * \brief Node of a Graph
	 * Can be inherited to add more functionality
	 */
	class GraphNode
	{
	public:
		GraphNode(bool canBeVisited = true);
		GraphNode(const glm::vec3& value, bool canBeVisited = true);
		~GraphNode() = default;
		GraphNode(const GraphNode& other);
		GraphNode(GraphNode&& other) noexcept;
		GraphNode& operator=(const GraphNode& other);
		GraphNode& operator=(GraphNode&& other) noexcept;

		float GetDistance(const GraphNode* neighbor) const;
		void AddNeighbor(GraphNode* neighbor, float distance, bool canVisit = false);
		glm::vec3 GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& value) { m_Position = value; }
		std::map<GraphNode*, GraphTransition> GetNeighbors() const { return m_Neighbors; }
		int GetId() const { return m_Id; }
		bool CanBeVisited() const { return m_CanBeVisited; }
		void SetTransitionCanBeVisited(GraphNode* neighbor);
		void SetCanBeVisited(bool value) { m_CanBeVisited = value; }
		bool IsNodeNeighbor(GraphNode* neighbor) const;
		bool GetTransitionCanBeVisited(GraphNode* neighbor) const;
		GraphNode* GetTopNeighbor() const;
		GraphNode* GetRightNeighbor() const;
		GraphNode* GetBottomNeighbor() const;
		GraphNode* GetLeftNeighbor() const;

	private:
		int m_Id;
		static int m_IdCounter;
		std::map<GraphNode*,GraphTransition> m_Neighbors;
		glm::vec3  m_Position;
		bool m_CanBeVisited;
	};

	class Graph final
	{
	public:
		Graph() = default;
		~Graph();
		Graph(const Graph& other);
		Graph(Graph&& other) noexcept;
		Graph& operator=(const Graph& other);
		Graph& operator=(Graph&& other) noexcept;

		GraphNode* AddNode();
		GraphNode* AddNode(const glm::vec3& value);
		GraphNode* GetNode(int index) const;
		GraphNode* GetNode(GraphNode* node);
		GraphNode* GetClosestNode(const glm::vec3& position) const;
		GraphNode* GetNextNode(GraphNode* pStart, const glm::vec2& direction);
		void RemoveNode(GraphNode* node);
		std::vector<GraphNode*> GetNodes() const;
		std::vector<GraphNode*> GetShortestPath(GraphNode* pStart, GraphNode* pEnd, bool canVisit = false);
	private:
		struct NodeRecord
		{
			GraphNode* pNode = nullptr;
			GraphNode* pFromNode = nullptr;
			float costSoFar = 0.f;
			float heuristicCost = 0.f;
			bool operator==(const NodeRecord& other) const;
			bool operator<(const NodeRecord& other) const;
		};
		std::vector<GraphNode*> m_Nodes;
		GraphNode* GetNodeMinDistance(const GraphUtils::GraphNode* endNode, const std::list<GraphUtils::GraphNode*>
		                              & openList, const std::list<GraphUtils::GraphNode*>& closedList);
		static float GetDistance(const GraphNode* start, const GraphNode* end);
		float GetHeuristic(const GraphNode* start, const GraphNode* end);
	};
}


