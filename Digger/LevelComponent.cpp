#include "LevelComponent.h"

#include "GameObject.h"
#include "Graph.h"
#include "imgui.h"
#include "SceneManager.h"

LevelComponent::LevelComponent() : BaseComponent(nullptr), m_pGraph{new GraphUtils::Graph{}}
{
	
}

LevelComponent::~LevelComponent()
{
	delete m_pGraph;
	m_pGraph = nullptr;
}

LevelComponent::LevelComponent(const LevelComponent& other): BaseComponent(other), m_pGraph{new GraphUtils::Graph{*other.m_pGraph}}
{
	
}

LevelComponent::LevelComponent(LevelComponent&& other) noexcept : BaseComponent{std::move(other)}, m_pGraph{other.m_pGraph}
{
	other.m_pGraph = nullptr;
}

LevelComponent& LevelComponent::operator=(const LevelComponent& other)
{
	if(this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(other);
	delete m_pGraph;
	m_pGraph = nullptr;
	m_ShortestPath.clear();
	m_pGraph = new GraphUtils::Graph{ *other.m_pGraph };
	return *this;
}

LevelComponent& LevelComponent::operator=(LevelComponent&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(std::move(other));
	delete m_pGraph;
	m_pGraph = nullptr;
	m_pGraph = other.m_pGraph;
	other.m_pGraph = nullptr;
	return *this;
}

std::shared_ptr<BaseComponent> LevelComponent::Clone() const
{
	return std::make_shared<LevelComponent>(*this);
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
			GraphUtils::GraphNode* current{ m_pGraph->AddNode(glm::vec3{ m_StartPos.x + 35 * static_cast<float>(j), m_StartPos.y + 35 * static_cast<float>(i), 0 }) };
			if (j != 0)
			{
				GraphUtils::GraphNode* left{ m_pGraph->GetNode(currentIndex - 1) };
				left->AddNeighbor(current, glm::distance(current->GetPosition(), left->GetPosition()));
				current->AddNeighbor(left, glm::distance(current->GetPosition(), left->GetPosition()));
			}
			const int indexTop{ currentIndex - maxColumn };
			if (i > 0)
			{
				GraphUtils::GraphNode* top{ m_pGraph->GetNode(indexTop) };
				top->AddNeighbor(current, glm::distance(current->GetPosition(), top->GetPosition()));
				current->AddNeighbor(top, glm::distance(current->GetPosition(), top->GetPosition()));
			}
		}
	}
	m_pGraph->GetNode(7)->SetCanBeVisited(false);
	m_pGraph->GetNode(21)->SetCanBeVisited(false);
	m_pGraph->GetNode(35)->SetCanBeVisited(false);
	m_pGraph->GetNode(49)->SetCanBeVisited(false);
	m_pGraph->GetNode(63)->SetCanBeVisited(false);

	m_ShortestPath = m_pGraph->GetShortestPath(m_pGraph->GetNode(8), m_pGraph->GetNode(6));
}

void LevelComponent::RenderGUI()
{
	dae::GameObject* player{ dae::SceneManager::GetInstance().GetGameObjectByTag("Player") };
	ImGui::Begin("Graph");
	for(size_t i{}; i < m_pGraph->GetNodes().size(); ++i)
	{
		const GraphUtils::GraphNode* node{ m_pGraph->GetNode(static_cast<int>(i)) };
		if(node->CanBeVisited())
			ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(node->GetPosition().x, node->GetPosition().y), 15, IM_COL32(255, 0, 0, 255));
		else
			ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(node->GetPosition().x, node->GetPosition().y), 15, IM_COL32(0, 255, 255, 255));
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
	if (player)
	{
		GraphUtils::GraphNode* closestNode{ m_pGraph->GetClosestNode(player->GetWorldPosition()) };
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(closestNode->GetPosition().x, closestNode->GetPosition().y), 15, IM_COL32(255, 255, 255, 255));
	}
	ImGui::End();
}

