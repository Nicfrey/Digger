#pragma once
#include <memory>

#include "Singleton.h"

namespace dae
{
	class GameObject;
}

class GameObjectComponent : public dae::Singleton<GameObjectComponent>
{
public:
	GameObjectComponent() = default;
	~GameObjectComponent() override;
	virtual void Update() = 0;
	virtual void Init() = 0;
	virtual void Render() const = 0;
	void SetGameObject(const std::shared_ptr<dae::GameObject>& go);

	GameObjectComponent(const GameObjectComponent& other) = delete;
	GameObjectComponent(GameObjectComponent&& other) noexcept = delete;
	GameObjectComponent& operator=(const GameObjectComponent& other) = delete;
	GameObjectComponent& operator=(GameObjectComponent&& other) noexcept = delete;
protected:
	std::weak_ptr<dae::GameObject> m_GameObject;
};

