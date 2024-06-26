#pragma once
#include "BaseComponent.h"

class EnemySpawnerComponent : public BaseComponent
{
public:
	EnemySpawnerComponent() = default;
	~EnemySpawnerComponent() override = default;
	EnemySpawnerComponent(const EnemySpawnerComponent& other) = default;
	EnemySpawnerComponent(EnemySpawnerComponent&& other) noexcept = default;
	EnemySpawnerComponent& operator=(const EnemySpawnerComponent& other) = default;
	EnemySpawnerComponent& operator=(EnemySpawnerComponent&& other) noexcept = default;
	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
	void Update() override;
	void OnDestroy() override;
	void CanSpawnPlayer();
private:
	void CreateNewEnemy();
	void SpawnNewEnemy();
	void DecreaseEnemyCount();
	void SetPlayerDead();
	void StopSpawning();
	int m_EnemyCount;
	int m_EnemyToSpawn{ 10 };
	bool m_PlayerDead{ false };
	bool m_CanSpawnPlayer{ false };
	std::shared_ptr<dae::GameObject> m_Player{ nullptr };
};

