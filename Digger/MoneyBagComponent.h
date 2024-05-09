#pragma once
#include <glm/vec2.hpp>

#include "BaseComponent.h"

class MoneyBagComponent : public BaseComponent
{
public:
	enum class StateMoneyBag
	{
		Idle,
		CanFall,
		IsFalling,
		IsDestroyed,
		IdleDestroyed
	};
	MoneyBagComponent() = default;
	~MoneyBagComponent() override = default;
	MoneyBagComponent(const MoneyBagComponent& other);
	MoneyBagComponent(MoneyBagComponent&& other) noexcept;
	MoneyBagComponent& operator=(const MoneyBagComponent& other);
	MoneyBagComponent& operator=(MoneyBagComponent&& other) noexcept;

	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
	void Update() override;
	void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) override;
private:
	StateMoneyBag m_State;
	int m_Score{ 500 };
	glm::vec2 m_Velocity{ 0.f,10.f };
};

