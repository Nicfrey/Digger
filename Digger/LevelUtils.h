#pragma once
#include "Graph.h"

class GraphNodeDigger : public GraphUtils::GraphNode
{
public:
	GraphNodeDigger();
	~GraphNodeDigger() = default;
	GraphNodeDigger(const glm::vec3& value);
	GraphNodeDigger(const GraphNodeDigger& other);
	GraphNodeDigger(GraphNodeDigger&& other) noexcept;
	GraphNodeDigger& operator=(const GraphNodeDigger& other);
	GraphNodeDigger& operator=(GraphNodeDigger&& other) noexcept;
private:

};
