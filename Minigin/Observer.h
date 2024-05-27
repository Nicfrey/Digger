#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "Singleton.h"

using EventID = std::string;
using EventHandler = std::function<void()>;

class IEventHandler
{
public:
	IEventHandler()	= default;
	virtual ~IEventHandler() = default;
	virtual void HandleEvent() const {}
	virtual bool Equals(IEventHandler* event) const
	{
		return event != nullptr;
	}
};

template<typename ClassType>
class EventHandlerMemberFunction : public IEventHandler
{
public:
	EventHandlerMemberFunction(ClassType* obj, void (ClassType::* funcPtr)())
		: m_pObject(obj)
		, m_pFunction(funcPtr)
	{
	}
	void HandleEvent() const override;
	bool Equals(IEventHandler* event) const override;

private:
	ClassType* m_pObject;
	void (ClassType::*m_pFunction)();
};

class EventHandlerFunction : public IEventHandler
{
public:
	EventHandlerFunction(const EventHandler& handler)
		: m_Handler(handler)
	{
	}
	void HandleEvent() const override;
	bool Equals(IEventHandler* event) const override;
private:
	EventHandler m_Handler;
};



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
	std::unordered_map<EventID, std::vector<std::unique_ptr<IEventHandler>>> m_EventHandler;
};

template <typename ClassType>
void EventHandlerMemberFunction<ClassType>::HandleEvent() const
{
	(m_pObject->*m_pFunction)();
}

template <typename ClassType>
bool EventHandlerMemberFunction<ClassType>::Equals(IEventHandler* event) const
{
	if(auto castedEvent = dynamic_cast<EventHandlerMemberFunction<ClassType>*>(event))
	{
		return m_pObject == castedEvent->m_pObject && m_pFunction == castedEvent->m_pFunction;
	}
	return false;
}


template <typename ClassType>
void EventManager::AddEvent(const EventID& eventID, ClassType* obj, void(ClassType::* funcPtr)())
{
	m_EventHandler[eventID].emplace_back(std::make_unique<EventHandlerMemberFunction<ClassType>>(obj, funcPtr));
}

template <typename ClassType>
void EventManager::RemoveEvent(const EventID& eventID, ClassType* obj, void(ClassType::* funcPtr)())
{
	const auto it = m_EventHandler.find(eventID);
	if (it != m_EventHandler.end())
	{
		auto& handlers = it->second;
		handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
			[obj, funcPtr](const std::unique_ptr<IEventHandler>& handler)
			{
				return handler->Equals(new EventHandlerMemberFunction<ClassType>(obj, funcPtr));
			}), handlers.end());
	}
}
