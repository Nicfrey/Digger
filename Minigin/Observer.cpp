#include "Observer.h"

void EventManager::AddEvent(const EventID& eventID, const EventHandler& handler)
{
	m_EventHandler[eventID].emplace_back(handler);
}

void EventManager::RemoveEvent(const EventID& eventID)
{
	m_EventHandler.erase(eventID);
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
