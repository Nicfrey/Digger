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
	LevelComponent(const LevelComponent& other);
	LevelComponent(LevelComponent&& other) noexcept;
	LevelComponent& operator=(const LevelComponent& other);
	LevelComponent& operator=(LevelComponent&& other) noexcept;
	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
	void RenderGUI() override;
private:
	GraphUtils::Graph* m_pGraph;
	std::vector<GraphUtils::GraphNode*> m_ShortestPath;
	glm::vec2 m_StartPos{20,20};
};

