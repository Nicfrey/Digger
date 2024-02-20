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

	virtual void Update() = 0;
	virtual void Init() = 0;
	virtual void Render() const = 0;
	void SetGameObject(const std::shared_ptr<dae::GameObject>& go);
protected:
	std::weak_ptr<dae::GameObject> m_GameObject;
};

