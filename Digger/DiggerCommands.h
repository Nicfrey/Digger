#pragma once
#include <glm/vec2.hpp>

#include "GameObjectCommand.h"
#include "LevelComponent.h"

namespace GraphUtils
{
	class GraphNode;
}

class MovePlayerCommand : public GameObjectCommand
{
public:
	MovePlayerCommand(dae::GameObject* go, glm::vec2 direction);
	void Execute() override;
protected:
	void SetCurrentAndTargetNode(const std::shared_ptr<LevelComponent>& levelComponent);
	glm::vec2 m_Direction;
	float m_Speed{80.f};
	float m_AcceptanceRadius{ 5.f };
	GraphUtils::GraphNode* m_CurrentNode;
	GraphUtils::GraphNode* m_TargetNode;
};

class MoveEnemyCommand : public MovePlayerCommand
{
	MoveEnemyCommand(dae::GameObject* go, glm::vec2 direction);
	void Execute() override;
};

class TransformEnemyCommand : public GameObjectCommand
{
public:
	TransformEnemyCommand(dae::GameObject* go);
	void Execute() override;
};

class ShootCommand : public GameObjectCommand
{
public:
	ShootCommand(dae::GameObject* go);
	void Execute() override;
};