#pragma once
#include <vector>

#include "BaseComponent.h"
#include <glm/vec2.hpp>

namespace GraphUtils
{
	class Graph;
	class GraphNode;
}

class LevelComponent : public BaseComponent
{
public:
	LevelComponent();
	~LevelComponent() override;
	void Init() override;
	void RenderGUI() override;
private:
	GraphUtils::Graph* m_Graph;
	std::vector<GraphUtils::GraphNode*> m_ShortestPath;
	glm::vec2 m_StartPos{20,20};
};

