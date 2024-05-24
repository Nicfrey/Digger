#pragma once
#include "BaseComponent.h"

class BackgroundComponent : public BaseComponent
{
public:
	~BackgroundComponent() override = default;
	std::shared_ptr<BaseComponent> Clone() const override;
	void OnCollisionEnter(std::shared_ptr<dae::GameObject>& other) override;
private:

};

