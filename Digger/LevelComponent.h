#pragma once
#define SIZE_LEVEL 150
#define SIZE_EDGE 1248
#include "BaseComponent.h"
#include <glm/vec2.hpp>
#include "lemon/list_graph.h"

class LevelComponent : public BaseComponent
{
public:
	LevelComponent();
	~LevelComponent() override = default;
	void Init() override;
	void RenderGUI() override;
private:

	lemon::ListGraph m_Graph{};
	lemon::ListGraph::Node m_Nodes[SIZE_LEVEL];
	glm::vec2 m_NodePositions[SIZE_LEVEL];
	glm::vec2 m_StartPos{20,20};

	bool IsCorner(int i, int j) const;
	bool IsEdge(int i, int j) const;
};

