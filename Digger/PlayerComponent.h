#pragma once
#include "BaseComponent.h"
#include "NavMeshAgentComponent.h"

class PlayerComponent : public BaseComponent
{
public:
	PlayerComponent();
	~PlayerComponent() override = default;
	PlayerComponent(const PlayerComponent& other);
	PlayerComponent(PlayerComponent&& other) noexcept;
	PlayerComponent& operator=(const PlayerComponent& other);
	PlayerComponent& operator=(PlayerComponent&& other) noexcept;
	std::shared_ptr<BaseComponent> Clone() const override;
	void Update() override;
	void Init() override;
	void HandleCollisionEnemy(const std::shared_ptr<dae::GameObject>& other) const;
	void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) override;
	void OnDestroy() override;
	void ShootProjectile();
	void ProjectileHasCollide();
	void HandleRespawn() const;
	GraphUtils::GraphNode*& GetPreviousNode();

private:
	dae::GameObject* m_pProjectile;
	std::unique_ptr<dae::GameObject> m_pDefaultProjectile;
	std::shared_ptr<NavMeshAgentComponent> m_NavMeshComp;
	void ResetProjectile();
	void HandleCollisionProjectile(std::shared_ptr<dae::GameObject>& other);
	std::shared_ptr<dae::GameObject> CreateProjectile() const;
	void HandleDeath();
	GraphUtils::GraphNode* m_pPreviousNode{ nullptr };
};

