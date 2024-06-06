#pragma once
#include "BaseComponent.h"

class NavMeshAgentComponent;

class EnemyComponent : public BaseComponent
{
public:
	enum class EnemyType
	{
		Nobbins,
		Hobbins
	};

	EnemyComponent();
	EnemyComponent(EnemyType type);
	void Init() override;
	void Update() override;
	std::shared_ptr<BaseComponent> Clone() const override;
	void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) override;
	void OnDestroy() override;
	EnemyType GetType() const;
	void IsPossessed();
	void Transform();
private:
	void HandleDeadEnemy();
	void StopMovingPlayerDead();
	void StopMoving();
	static int m_ScoreKilled;
	EnemyType m_Type;
	NavMeshAgentComponent* m_pNavMeshAgent;
	bool m_StopMoving{ false };
	bool m_IsPossessed{ false };
	bool m_CanTransform{ true };
};

