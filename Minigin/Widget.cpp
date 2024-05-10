#include "Widget.h"

#include <iostream>

#include "ButtonComponent.h"

Widget::Widget(const std::string& name): m_Name{name}
{
}

Widget::Widget(Widget&& other) noexcept
{
	m_Elements = std::move(other.m_Elements);
}

Widget& Widget::operator=(const Widget& other)
{
	if (this == &other)
	{
		return *this;
	}
	m_Elements = other.m_Elements;
	return *this;
}

Widget& Widget::operator=(Widget&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	m_Elements = std::move(other.m_Elements);
	return *this;
}

bool Widget::RemoveComponent(int index)
{
	if(index < 0 || static_cast<size_t>(index) >= m_Elements.size())
	{
		std::cerr << index << " is out of range of element Widget\n";
		return false;
	}
	m_Elements.erase(m_Elements.begin() + index);
	return true;
}

void Widget::HandleOnClickEvent(const glm::vec2& posMouse) const
{
	if(!HasElement<ButtonComponent>())
	{
		return;
	}
	auto buttons{ GetAllElement<ButtonComponent>() };
	for (auto& button : buttons)
	{
		button->OnClick(posMouse);
	}
}

void Widget::HandleOnPressedEvent() const
{
	if (!HasElement<ButtonComponent>())
	{
		return;
	}
	auto buttons{ GetAllElement<ButtonComponent>() };
	for (auto& button : buttons)
	{
		button->OnPressed();
	}
}

void Widget::Render() const
{
	for (auto& element : m_Elements)
	{
		element->RenderElement();
	}
}
