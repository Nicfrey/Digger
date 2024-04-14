#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;
}

class GameObjectCommand : public Command
{
public:
	GameObjectCommand() = default;
	GameObjectCommand(dae::GameObject* go);
	~GameObjectCommand() override = default;

	GameObjectCommand(const GameObjectCommand& other) = delete;
	GameObjectCommand(GameObjectCommand&& other) noexcept = delete;
	GameObjectCommand& operator=(const GameObjectCommand& other) = delete;
	GameObjectCommand& operator=(GameObjectCommand&& other) noexcept = delete;
protected:
	dae::GameObject* GetGameObject() const;
private:
	dae::GameObject* m_GameObject;
};

