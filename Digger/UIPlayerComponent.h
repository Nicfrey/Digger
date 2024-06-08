#pragma once
#include "TextComponent.h"

class Widget;

class UIPlayerComponent : public BaseComponent
{
public:
	UIPlayerComponent() = default;
	~UIPlayerComponent() override = default;
	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
	void OnDestroy() override;
private:
	void UpdateScore();
	void UpdateLives();
	std::shared_ptr<Widget> m_WidgetUI;
};

