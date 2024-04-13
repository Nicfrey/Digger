#include "LevelComponent.h"

#include <glm/detail/func_geometric.inl>

#include "imgui.h"

LevelComponent::LevelComponent() : BaseComponent(nullptr), m_NodePositions {}
{
	
}


void LevelComponent::Init()
{
	lemon::ListGraph g;
	lemon::ListGraph::EdgeMap<float> edgeDistance{ g };
	const int maxColumn{ 15 };
	int edgeIndex{};
	for(int i{}; i < 10; ++i)
	{
		for (int j{}; j < maxColumn; ++j)
		{
			const int currentIndex{ i * j + i };
			m_Nodes[currentIndex] = m_Graph.addNode();
			m_NodePositions[currentIndex] = glm::vec2( m_StartPos.x + m_StartPos.x * static_cast<float>(j), m_StartPos.y + m_StartPos.y * static_cast<float>(i) );
			const int indexTop{ currentIndex - j };
			if (i != 0 && j != 0)
			{
				lemon::ListGraph::Edge edge = m_Graph.addEdge(m_Nodes[currentIndex], m_Nodes[currentIndex - 1]);
				edgeDistance[edge] = glm::distance(m_NodePositions[currentIndex], m_NodePositions[currentIndex - 1]);
				++edgeIndex;
			}
			if(indexTop >= 0)
			{
				lemon::ListGraph::Edge edge = m_Graph.addEdge(m_Nodes[currentIndex], m_Nodes[indexTop]);
				edgeDistance[edge] = glm::distance(m_NodePositions[currentIndex], m_NodePositions[indexTop]);
				++edgeIndex;
			}
		}
	}
}

void LevelComponent::RenderGUI()
{
	ImGui::Begin("Graph");
	for(int i{}; i < SIZE_LEVEL; ++i)
	{
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(m_NodePositions[i].x, m_NodePositions[i].y), 5, IM_COL32(255, 0, 0, 255));
	}
	ImGui::End();
}

bool LevelComponent::IsCorner(int i, int j) const
{
	return (i == 0 && j == 0) || (i == 0 && j == 14) || (i == 9 && j == 0) || (i == 9 && j == 14);
}

bool LevelComponent::IsEdge(int i, int j) const
{
	return i == 0 || j == 0 || i == 9 || j == 14;
}
