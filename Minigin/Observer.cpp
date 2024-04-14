#include "Observer.h"

void EventManager::AddEvent(const EventID& eventID, const EventHandler& handler)
{
	m_EventHandler[eventID].emplace_back(handler);
}

void EventManager::RemoveEvent(const EventID& eventID, const EventHandler& handler)
{
	const auto it = m_EventHandler.find(eventID);
	if (it != m_EventHandler.end())
	{
		auto& handlers = it->second;
		for (auto itHandler = handlers.begin(); itHandler != handlers.end(); ++itHandler)
		{
			if (itHandler->target<void()>() == handler.target<void()>())
			{
				handlers.erase(itHandler);
				break;
			}
		}
	}
}

void EventManager::NotifyEvent(const EventID& eventID) const
{
	if(m_EventHandler.contains(eventID))
	{
		for(const auto& pair: m_EventHandler)
		{
			if(pair.first == eventID)
			{
				for (const auto& handler : pair.second)
				{
					handler();
				}
			}
		}
	}
}
