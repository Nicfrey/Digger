#pragma once
#include <functional>
#include <string>
#include <unordered_map>

#include "Singleton.h"

typedef std::string EventID;
using EventHandler = std::function<void()>;

class EventManager : public dae::Singleton<EventManager>
{
public:
	template<typename ClassType>
	void AddEvent(const EventID& eventID, ClassType* obj, void (ClassType::* funcPtr)());
	void AddEvent(const EventID& eventID, const EventHandler& handler);
	template<typename ClassType>
	void RemoveEvent(const EventID& eventID, ClassType* obj, void (ClassType::* funcPtr)());
	void RemoveEvent(const EventID& eventID, const EventHandler& handler);
	void NotifyEvent(const EventID& eventID) const;
private:
	std::unordered_map<EventID, std::vector<EventHandler>> m_EventHandler;
};

template <typename ClassType>
void EventManager::AddEvent(const EventID& eventID, ClassType* obj, void(ClassType::* funcPtr)())
{
		AddEvent(eventID, [obj, funcPtr]()
		{
			(obj->*funcPtr)();
		});
}

template <typename ClassType>
void EventManager::RemoveEvent(const EventID& eventID, ClassType* obj, void(ClassType::* funcPtr)())
{
	const auto it = m_EventHandler.find(eventID);
	if (it != m_EventHandler.end())
	{
		auto& handlers = it->second;
		for (auto itHandler = handlers.begin(); itHandler != handlers.end(); ++itHandler)
		{
			std::function<void()> handler = [obj, funcPtr]()
			{
				(obj->*funcPtr)();
			};
			if (itHandler->target<void()>() == handler.target<void()>())
			{
				handlers.erase(itHandler);
				break;
			}
		}
	}
}
