#pragma once
#include "BaseComponent.h"

class ItemComponent : public BaseComponent
{
public:
	enum class ItemType
	{
		Emerald,
		Gold,
		GoldNugget
	};
	ItemComponent() = default;
	ItemComponent(ItemType type);
	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
	ItemType GetType() const { return m_Type; }
private:
	ItemType m_Type;
};

