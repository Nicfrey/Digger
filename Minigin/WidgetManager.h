#pragma once
#include <memory>
#include <string>

#include "Singleton.h"
#include <vector>
#include <glm/vec3.hpp>

class Widget;

class WidgetManager : public dae::Singleton<WidgetManager>
{
public:
	WidgetManager() = default;
	void AddWidget(const std::shared_ptr<Widget>& widget);
	void SetActiveWidget(const std::shared_ptr<Widget>& widget);
	void SetActiveWidget(int index);
	void SetActiveWidget(const std::string& name);
	void RemoveActiveWidget();
	void Render() const;
	void Update();
	void HandleOnClickEvent(const glm::vec3& vec);
	std::shared_ptr<Widget> GetActiveWidget();
	void HandleOnHoverEvent(const glm::vec3& vec);
	std::shared_ptr<Widget> GetWidget(const std::string& name);

private:
	bool HasElementWithName(const std::string& name) const;
	std::vector<std::shared_ptr<Widget>> m_Widgets;
	std::shared_ptr<Widget> m_ActiveWidget;
};

