#pragma once
#include <glm/vec2.hpp>

#include "GameObjectCommand.h"

namespace GraphUtils
{
	class GraphNode;
}

class MoveCommand : public GameObjectCommand
{
public:
	MoveCommand(dae::GameObject* go, glm::vec2 direction);
	void Execute() override;
private:
	glm::vec2 m_Direction;
	float m_Speed{80.f};
	float m_AcceptanceRadius{ 5.f };
	GraphUtils::GraphNode* m_CurrentNode;
	GraphUtils::GraphNode* m_TargetNode;
};

class ShootCommand : public GameObjectCommand
{
public:
	ShootCommand(dae::GameObject* go);
	void Execute() override;
};