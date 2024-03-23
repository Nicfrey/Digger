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
	void AddEvent(const EventID& eventID, const EventHandler& handler);
	void RemoveEvent(const EventID& eventID);
	void NotifyEvent(const EventID& eventID) const;
private:
	std::unordered_map<EventID, std::vector<EventHandler>> m_EventHandler;
};
