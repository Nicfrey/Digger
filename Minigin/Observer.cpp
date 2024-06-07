#include "Observer.h"

void EventHandlerFunction::HandleEvent() const
{
	m_Handler();
}

bool EventHandlerFunction::Equals(IEventHandler* event) const
{
	if (auto* p = dynamic_cast<EventHandlerFunction*>(event))
	{
		return m_Handler.target<void()>() == p->m_Handler.target<void()>();
	}
	return false;
}

void EventManager::AddEvent(const EventID& eventID, const EventHandler& handler)
{
	m_EventHandler[eventID].emplace_back(std::make_unique<EventHandlerFunction>(handler));
}

void EventManager::RemoveEvent(const EventID& eventID, const EventHandler& handler)
{
	const auto it = m_EventHandler.find(eventID);
	if (it != m_EventHandler.end())
	{
		auto& handlers = it->second;
		std::erase_if(handlers,
		              [&handler](const std::unique_ptr<IEventHandler>& eventHandler)
		              {
			              return eventHandler->Equals(new EventHandlerFunction(handler));
		              });
	}
}

void EventManager::NotifyEvent(const EventID& eventID) const
{
	const auto it = m_EventHandler.find(eventID);
	if (it != m_EventHandler.end())
	{
		for (const auto& handler : it->second)
		{
			handler->HandleEvent();
		}
	}
}
