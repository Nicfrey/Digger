#pragma once
#include "BaseComponent.h"

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
	std::shared_ptr<BaseComponent> Clone() const override;
	void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) override;
	void OnEnemyDead();
	void OnDestroy() override;

private:
	static int m_ScoreKilled;
	EnemyType m_Type;
};

