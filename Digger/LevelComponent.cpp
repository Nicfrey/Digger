#include "LevelComponent.h"

#include <glm/detail/func_geometric.inl>

#include "Graph.h"
#include "imgui.h"

LevelComponent::LevelComponent() : BaseComponent(nullptr), m_Graph{new GraphUtils::Graph{}}
{
	
}

LevelComponent::~LevelComponent()
{
	delete m_Graph;
	m_Graph = nullptr;
}


void LevelComponent::Init()
{
	constexpr int maxRow{ 10 };
	constexpr int maxColumn{ 15 };
	for (int i{}; i < maxRow; ++i)
	{
		for (int j{}; j < maxColumn; ++j)
		{
			const int currentIndex{ i * maxColumn + j };
			GraphUtils::GraphNode* current{ m_Graph->AddNode(glm::vec3{ m_StartPos.x + 35 * static_cast<float>(j), m_StartPos.y + 35 * static_cast<float>(i), 0 }) };
			if (j != 0)
			{
				GraphUtils::GraphNode* left{ m_Graph->GetNode(currentIndex - 1) };
				left->AddNeighbor(current, glm::distance(current->GetPosition(), left->GetPosition()));
				current->AddNeighbor(left, glm::distance(current->GetPosition(), left->GetPosition()));
			}
			const int indexTop{ currentIndex - maxColumn };
			if (i > 0)
			{
				GraphUtils::GraphNode* top{ m_Graph->GetNode(indexTop) };
				top->AddNeighbor(current, glm::distance(current->GetPosition(), top->GetPosition()));
				current->AddNeighbor(top, glm::distance(current->GetPosition(), top->GetPosition()));
			}
		}
	}
	m_ShortestPath = m_Graph->GetShortestPath(m_Graph->GetNode(25), m_Graph->GetNode(149));
}

void LevelComponent::RenderGUI()
{
	ImGui::Begin("Graph");
	for(size_t i{}; i < m_Graph->GetNodes().size(); ++i)
	{
		const GraphUtils::GraphNode* node{ m_Graph->GetNode(i) };
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(node->GetPosition().x, node->GetPosition().y), 15, IM_COL32(255, 0, 0, 255));
		for (const auto& neighbor : node->GetNeighbors())
		{
			const GraphUtils::GraphNode* neighborNode{ neighbor.first };
			ImGui::GetWindowDrawList()->AddLine(ImVec2(node->GetPosition().x, node->GetPosition().y), ImVec2(neighborNode->GetPosition().x, neighborNode->GetPosition().y), IM_COL32(0, 255, 0, 255));
		}
	}
	for(size_t i{}; i < m_ShortestPath.size(); ++i)
	{
		const GraphUtils::GraphNode* node{ m_ShortestPath[i] };
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(node->GetPosition().x, node->GetPosition().y), 15, IM_COL32(0, 255, 0, 255));
	}
	ImGui::End();
}
