#pragma once
#include <glm/vec3.hpp>

#include "BaseComponent.h"
#include "Graph.h"

class BackgroundComponent : public BaseComponent
{
public:
	~BackgroundComponent() override = default;
	std::shared_ptr<BaseComponent> Clone() const override;
	void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) override;
private:
	void HandlePlayerCollision(std::shared_ptr<dae::GameObject>& other) const;
	void HandleProjectileCollision(std::shared_ptr<dae::GameObject>& other);
	void HandleEnemyCollision(std::shared_ptr<dae::GameObject>& other) const;
};

