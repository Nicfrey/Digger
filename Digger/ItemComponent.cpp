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
}
