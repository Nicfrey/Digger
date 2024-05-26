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
	void OnDestroy() override;
private:
	void CreateNewEnemy() const;
	void SpawnNewEnemy();
	void DecreaseEnemyCount();
	int m_EnemyCount;
	int m_EnemyToSpawn{ 10 };
	float m_SpawnRate{ 5.f };
};

