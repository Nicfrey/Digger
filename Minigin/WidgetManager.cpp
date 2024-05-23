#include "WidgetManager.h"

#include <iostream>

#include "Widget.h"

void WidgetManager::AddWidget(const std::shared_ptr<Widget>& widget)
{
	if (HasElementWithName(widget->GetName()))
	{
		std::cerr << "Widget with name " << widget->GetName() << " already exists\n";
		return;
	}
	m_Widgets.emplace_back(widget);
	if(m_ActiveWidget == nullptr)
	{
		m_ActiveWidget = widget;
	}
}

void WidgetManager::SetActiveWidget(const std::shared_ptr<Widget>& widget)
{
	const auto it{ std::ranges::find_if(m_Widgets, [widget](const std::shared_ptr<Widget>& other)
	{
		return widget.get() == other.get();
	})};
	if (it != m_Widgets.end())
	{
		m_ActiveWidget = *it;
	}
}

void WidgetManager::SetActiveWidget(int index)
{
	if (index < 0 || static_cast<size_t>(index) >= m_Widgets.size())
	{
		std::cerr << index << " is out of range of element Widget\n";
		return;
	}
	m_ActiveWidget = m_Widgets[index];
}

void WidgetManager::SetActiveWidget(const std::string& name)
{
	if(!HasElementWithName(name))
	{
		std::cerr << "Widget with name '" << name << "' does not exist\n";
		return;
	}
	const auto it{ std::ranges::find_if(m_Widgets, [name](const std::shared_ptr<Widget>& other)
	{
		return other->GetName() == name;
	}) };
	if(it != m_Widgets.end())
	{
		m_ActiveWidget = *it;
	}
}

void WidgetManager::RemoveActiveWidget()
{
	m_ActiveWidget = nullptr;
}

void WidgetManager::Render() const
{
	if (m_ActiveWidget != nullptr)
	{
		m_ActiveWidget->Render();
	}
}

void WidgetManager::HandleOnClickEvent(const glm::vec3& vec)
{
	if (m_ActiveWidget != nullptr)
	{
		m_ActiveWidget->HandleOnClickEvent(vec);
	}
}

bool WidgetManager::HasElementWithName(const std::string& name) const
{
	const auto it{ std::ranges::find_if(m_Widgets, [name](const std::shared_ptr<Widget>& other)
	{
		return other->GetName() == name;
	})};
	return it != m_Widgets.end();
}
