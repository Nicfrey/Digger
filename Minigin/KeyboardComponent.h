#pragma once
#include "UIElementComponent.h"

class KeyboardComponent final : public UIElementComponent
{
public:
	KeyboardComponent() = default;
	~KeyboardComponent() override = default;
	void RenderElement() const override;
private:

};

