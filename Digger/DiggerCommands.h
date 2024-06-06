#pragma once
#include <glm/vec2.hpp>

#include "GameObjectCommand.h"
#include "LevelComponent.h"

class ButtonComponent;

namespace GraphUtils
{
	class GraphNode;
}

#pragma region PlayerCommands
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

class ShootCommand : public GameObjectCommand
{
public:
	ShootCommand(dae::GameObject* go);
	void Execute() override;
};

#pragma endregion PlayerCommands

#pragma region PlayerCommands

class MoveEnemyCommand : public MovePlayerCommand
{
public:
	MoveEnemyCommand(dae::GameObject* go, glm::vec2 direction);
	void Execute() override;
};

class TransformEnemyCommand : public GameObjectCommand
{
public:
	TransformEnemyCommand(dae::GameObject* go);
	void Execute() override;
};

#pragma endregion PlayerCommands

#pragma region UICommands

class MoveButtonCommand : public Command
{
public:
	MoveButtonCommand(bool isUp);
	void Execute() override;
private:
	bool m_Up;
	static void SelectFirstIfNoSelected(const std::vector<std::shared_ptr<ButtonComponent>>& buttons);
};

class SelectButtonCommand : public Command
{
public:
	SelectButtonCommand() = default;
	void Execute() override;
private:
	void HandleButtonWidget();
	void HandleButtonKeyboard();
};

class MoveKeyboardCommand : public Command
{
public:
	MoveKeyboardCommand(const glm::ivec2& direction);
	void Execute() override;
private:
	glm::ivec2 m_Direction;
};

#pragma endregion UICommands

#pragma region OtherCommands
class SkipLevelCommand : public Command
{
public:
	SkipLevelCommand() = default;
	void Execute() override;
};

class MuteCommand : public Command
{
public:
	MuteCommand() = default;
	void Execute() override;
};
#pragma endregion OtherCommands



