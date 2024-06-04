#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <glm/vec2.hpp>

#include "BaseComponent.h"
#include "UIElementComponent.h"

class UIElementComponent;
class ButtonComponent;

class Widget final
{
public:
	Widget(const std::string& name);
	~Widget() = default;
	Widget(const Widget& other) = default;
	Widget(Widget&& other) noexcept;
	Widget& operator=(const Widget& other);
	Widget& operator=(Widget&& other) noexcept;
	template<typename ClassType>
	bool AddElement(const std::shared_ptr<ClassType>& element);
	template<typename ClassType>
	bool RemoveComponent(const std::shared_ptr<ClassType>& element);
	bool RemoveComponent(int index);
	template<typename ClassType>
	std::shared_ptr<ClassType> GetElement() const;
	template<typename ClassType>
	std::shared_ptr<ClassType> GetElement(const std::string& name) const;
	template<typename ClassType>
	std::vector<std::shared_ptr<ClassType>> GetAllElement() const;
	template<typename ClassType>
	bool HasElement() const;
	void HandleOnClickEvent(const glm::vec2& posMouse) const;
	void HandleOnPressedEvent() const;
	void Render() const;
	std::string GetName() const { return m_Name; }
private:
	void HandleButtons(const glm::vec2& posMouse) const;
	void HandleKeyboard(const glm::vec2& posMouse) const;
	std::vector<std::shared_ptr<UIElementComponent>> m_Elements;
	std::string m_Name;
};

template <typename ClassType>
bool Widget::AddElement(const std::shared_ptr<ClassType>& element)
{
	if(std::shared_ptr<UIElementComponent> uiElementCaster{std::dynamic_pointer_cast<UIElementComponent>(element)})
	{
		if (GetElement<ClassType>(uiElementCaster->GetName()))
		{
			std::cerr << "Element with name " << uiElementCaster->GetName() << " already exists in Widget\n";
			return false;
		}
		m_Elements.emplace_back(element);
		return true;
	}
	return false;
}

template <typename ClassType>
bool Widget::RemoveComponent(const std::shared_ptr<ClassType>& element)
{
	auto it{ std::find_if(m_Elements.begin(), m_Elements.end(), [element](const std::shared_ptr<UIElementComponent>& other)
	{
		return element.get() == other.get();
	}) };
	if(it != m_Elements.end())
	{
		m_Elements.erase(it);
		return true;
	}
	return false;
}

template <typename ClassType>
std::shared_ptr<ClassType> Widget::GetElement() const
{
	for (const std::shared_ptr<UIElementComponent>& elements : m_Elements)
	{
		std::shared_ptr<ClassType> elementGot{ std::dynamic_pointer_cast<ClassType>(elements) };
		if (elementGot != nullptr)
		{
			return elementGot;
		}
	}
	return std::shared_ptr<ClassType>{nullptr};
}

template <typename ClassType>
std::shared_ptr<ClassType> Widget::GetElement(const std::string& name) const
{
	for (const std::shared_ptr<UIElementComponent>& elements : m_Elements)
	{
		std::shared_ptr<ClassType> elementGot{ std::dynamic_pointer_cast<ClassType>(elements) };
		if (elementGot != nullptr && elementGot->GetName() == name)
		{
			return elementGot;
		}
	}
	return std::shared_ptr<ClassType>{nullptr};
}

template <typename ClassType>
std::vector<std::shared_ptr<ClassType>> Widget::GetAllElement() const
{
	std::vector<std::shared_ptr<ClassType>> result{};
	for(const std::shared_ptr<UIElementComponent>& element: m_Elements)
	{
		std::shared_ptr<ClassType> elementGot{ std::dynamic_pointer_cast<ClassType>(element) };
		if(elementGot != nullptr)
		{
			result.emplace_back(elementGot);
		}
	}
	return result;
}

template <typename ClassType>
bool Widget::HasElement() const
{
	return GetElement<ClassType>() != nullptr;
}
