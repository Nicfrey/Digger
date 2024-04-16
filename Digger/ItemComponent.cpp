#include "ItemComponent.h"

#include "GameObject.h"
#include "SpriteComponent.h"

ItemComponent::ItemComponent(ItemType type): BaseComponent(nullptr), m_Type{type}
{
}

std::shared_ptr<BaseComponent> ItemComponent::Clone() const
{
	return std::make_shared<ItemComponent>(*this);
}

void ItemComponent::Init()
{
	if (GetGameObject()->HasComponent<SpriteComponent>())
	{
		const auto sprite{GetGameObject()->GetComponent<SpriteComponent>()};
		switch (m_Type)
		{
		case ItemType::Emerald:
			sprite->SetCurrentRow(2);
			break;
		case ItemType::Gold:
			sprite->SetCurrentRow(0);
			sprite->SetCurrentCol(1);
			break;
		case ItemType::GoldNugget:
			sprite->SetCurrentRow(1);
			break;
		}
	}
}
