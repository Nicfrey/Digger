#pragma once
#include <glm/vec2.hpp>

#include "GameObjectCommand.h"

class MoveCommand : public GameObjectCommand
{
public:
	MoveCommand(dae::GameObject* go, glm::vec2 direction);
	void Execute() override;
private:
	glm::vec2 m_Direction;
	float m_Speed{80.f};
};
