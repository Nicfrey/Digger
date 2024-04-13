#pragma once
#include <list>
#include <map>
#include <SDL_opengl.h>
#include <vector>
#include <glm/vec3.hpp>

namespace GraphUtils
{
	class GraphNode final
	{
	public:
		GraphNode();
		GraphNode(const glm::vec3& value);
		~GraphNode();
		float GetDistance(const GraphNode* neighbor) const;
		void AddNeighbor(GraphNode* neighbor, float distance);
		glm::vec3 GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& value) { m_Position = value; }
		std::map<GraphNode*, float> GetNeighbors() const { return m_Neighbors; }
		int GetId() const { return m_Id; }

	private:
		int m_Id;
		static int m_IdCounter;
		std::map<GraphNode*,float> m_Neighbors;
		glm::vec3  m_Position;
	};

	class Graph final
	{
	public:
		Graph() = default;
		~Graph();
		GraphNode* AddNode();
		GraphNode* AddNode(const glm::vec3& value);
		GraphNode* GetNode(int index) const;
		GraphNode* GetNode(GraphNode* node);
		void RemoveNode(GraphNode* node);
		std::vector<GraphNode*> GetNodes() const;
		std::vector<GraphNode*> GetShortestPath(GraphNode* pStart, GraphNode* pEnd);
	private:
		std::vector<GraphNode*> m_Nodes;
		GraphNode* GetNodeMinDistance(const GraphUtils::GraphNode* endNode, const std::list<GraphUtils::GraphNode*>
		                              & openList);
		float GetDistance(const GraphNode* start, const GraphNode* end);
	};
}


