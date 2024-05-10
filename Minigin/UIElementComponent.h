#pragma once
#include <string>

class UIElementComponent
{
public:
	UIElementComponent(const std::string& name);
	virtual ~UIElementComponent() = default;
	UIElementComponent(const UIElementComponent& other) = default;
	UIElementComponent(UIElementComponent&& other) noexcept = default;
	UIElementComponent& operator=(const UIElementComponent& other) = default;
	UIElementComponent& operator=(UIElementComponent&& other) noexcept = default;
	virtual void RenderElement() const {}
	std::string GetName() const { return m_Name; }
private:
	std::string m_Name;
};

