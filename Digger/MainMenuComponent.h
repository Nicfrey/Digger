#pragma once
#include "BaseComponent.h"

class LevelComponent;

class MainMenuComponent final : public BaseComponent
{
public:
	MainMenuComponent() = default;
	~MainMenuComponent() override = default;
	MainMenuComponent(const MainMenuComponent& other) = default;
	MainMenuComponent(MainMenuComponent&& other) noexcept = default;
	MainMenuComponent& operator=(const MainMenuComponent& other) = default;
	MainMenuComponent& operator=(MainMenuComponent&& other) noexcept = default;
	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
private:
	std::shared_ptr<LevelComponent> m_LevelComponent;
};

