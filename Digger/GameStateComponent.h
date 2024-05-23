#pragma once
#include "BaseComponent.h"
#include "FiniteStateMachine.h"


using DiggerStateMachine = FiniteStateMachine;

class GameStateComponent : public BaseComponent
{
public:
	GameStateComponent();
	~GameStateComponent() override = default;
	GameStateComponent(const GameStateComponent& other);
	GameStateComponent(GameStateComponent&& other) noexcept;
	GameStateComponent& operator=(const GameStateComponent& other);
	GameStateComponent& operator=(GameStateComponent&& other) noexcept;
	void Update() override;
	
	std::shared_ptr<BaseComponent> Clone() const override;
private:
	std::unique_ptr<DiggerStateMachine> m_StateMachine;
};

