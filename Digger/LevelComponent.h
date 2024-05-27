#pragma once
#include <vector>

#include "BaseComponent.h"
#include <glm/vec2.hpp>

#include "json.hpp"


namespace DiggerUtils
{
	enum class DiggerGameMode;
}

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
	GraphUtils::Graph* GetGraph() const;
	LevelComponent(const LevelComponent& other);
	LevelComponent(LevelComponent&& other) noexcept;
	LevelComponent& operator=(const LevelComponent& other);
	LevelComponent& operator=(LevelComponent&& other) noexcept;
	std::shared_ptr<BaseComponent> Clone() const override;
	void Update() override;
	void Init() override;
	void RenderGUI() override;
private:
	DiggerUtils::DiggerGameMode m_GameMode{};
	GraphUtils::Graph* m_pGraph;
	std::vector<GraphUtils::GraphNode*> m_ShortestPath;
	glm::vec2 m_StartPos{20,20};
	glm::vec2 m_SpawnPointEnemy;
	std::vector<std::shared_ptr<dae::GameObject>> m_Players;

	void CreateSpawnerEnemy(int index) const;
	void LoadLevel();
	glm::vec2 GetVectorFromJson(const nlohmann::json& json);
	void CreateEmeraldAtIndex(int index);
	void CreateMoneyBagAtIndex(int index);
	void CreateBackgroundLevel(int level);
	void CreatePlayerAtIndex(int index, int player);
	int GetIndexFromPosition(const glm::vec2& pos, int maxColumn);
	void UpdateGraph();
};

