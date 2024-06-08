#pragma once
#include <vector>

#include "BaseComponent.h"
#include <glm/vec2.hpp>

#include "json.hpp"


class ThreadPool;

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
	LevelComponent(const LevelComponent& other);
	LevelComponent(LevelComponent&& other) noexcept;
	LevelComponent& operator=(const LevelComponent& other);
	LevelComponent& operator=(LevelComponent&& other) noexcept;
	std::shared_ptr<BaseComponent> Clone() const override;
	bool IsNodeMoneyBag(const GraphUtils::GraphNode* node) const;
	GraphUtils::Graph* GetGraph() const;
	void RenderGUI() override;
	void OnDestroy() override;
	void FreeSpaceMoneyBag(GraphUtils::GraphNode* node) const;
private:
	DiggerUtils::DiggerGameMode m_GameMode{};
	int m_Level{};
	int m_MaxLevel{ 3 };
	int m_MaxColumn{ 15 };
	int m_MaxRow{ 10 };
	GraphUtils::Graph* m_pGraph;
	std::vector<GraphUtils::GraphNode*> m_ShortestPath;
	glm::vec2 m_StartPos{20,20};
	std::unique_ptr<ThreadPool> m_pThreadPool;

	void CreateSpawnerEnemy(int index) const;
	void LoadLevel();
	void InitializeLevel(const nlohmann::json& json);
	void RespawnPlayers();
	glm::vec2 GetVectorFromJson(const nlohmann::json& json);
	void CreateEmeraldAtIndex(int index);
	void CreateMoneyBagAtIndex(int index);
	void CreateBackgroundLevel(int level);
	void CreatePlayerAtIndex(int index, int player);
	int GetIndexFromPosition(const glm::vec2& pos, int maxColumn);
	void UpdateGraphPlayers();
	void UpdateGraphEnemies();
	void InitializeGraph(const nlohmann::json& json) const;
	void CheckRemainingEmeralds();
	void CheckRemainingEnemies();
	void CreateUIObject();
	void HandleFreeSpace(GraphUtils::GraphNode*& previousNode, const std::shared_ptr<dae::GameObject>& object);
};

