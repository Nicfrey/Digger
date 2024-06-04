#pragma once
#include <functional>
#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Singleton.h"
#include "TimeEngine.h"

#pragma region structs

namespace dae
{
	class GameObject;
}

	struct Rectf final
	{
		Rectf() = default;
		Rectf(glm::vec2 topLeft, float width, float height);
		Rectf(float width, float height);
		glm::vec2 topLeft;
		float width;
		float height;
		glm::vec2 GetCenter() const;
	};

	struct Circlef final
	{
		glm::vec2 center{};
		float radius{0.f};
	};

	struct Linef final
	{
		glm::vec2 p0{0.f,0.f};
		glm::vec2 p1{0.f,0.f};
	};

	struct RaycastHitInfo2D
	{
		glm::vec2 point;
		glm::vec2 normal;
		float distance;
		dae::GameObject* pGameObject;
	};

	struct Color3f final
	{
		int r{};
		int g{};
		int b{};
	};

	struct Color4f final
	{
		int r{};
		int g{};
		int b{};
		int a{ 255 };
	};

	struct Timer
	{
		float timer{};
		float currentTimer{};
		bool repeat{};
	};

	using DelegateFnc = std::function<void()>;

	class ITimerHandler
	{
	public:
		virtual ~ITimerHandler() = default;
		virtual void HandleTimer() = 0;
		virtual bool Equals(ITimerHandler* timer) = 0;
		virtual bool IsDone() const = 0;
	};

	template<typename ClassType>
	class TimerHandlerMember final : public ITimerHandler
	{
	public:
		TimerHandlerMember(ClassType* obj, void (ClassType::* funcPtr)(), float timer, bool repeat = false);
		void HandleTimer() override;
		bool Equals(ITimerHandler* timer) override;
		bool IsDone() const override;
	private:
		ClassType* m_pObject;
		void (ClassType::* m_pFunction)();
		Timer m_Timer;
	};

	class TimerHandlerFunction final : public ITimerHandler
	{
	public:
		TimerHandlerFunction(const DelegateFnc& func, float timer,bool repeat = false);
		void HandleTimer() override;
		bool Equals(ITimerHandler* timer) override;
		bool IsDone() const override;
	private:
		DelegateFnc m_Handler;
		Timer m_Timer;
	};

	class TimerManager final : public dae::Singleton<TimerManager>
	{
	public:
		template<typename ClassType>
		void AddTimer(ClassType* obj, void (ClassType::* funcPtr)(),float timer, bool repeat = false);
		void AddTimer(const DelegateFnc& function, float timer, bool repeat = false);
		void Update();
		template<typename ClassType>
		void RemoveTimer(ClassType* obj, void(ClassType::* funcPtr)(),float timer);
		void RemoveTimer(const DelegateFnc& function, float timer);
	private:
		std::vector<std::unique_ptr<ITimerHandler>> m_TimerHandlers;
		void RemoveTimerDone();
	};

template <typename ClassType>
TimerHandlerMember<ClassType>::TimerHandlerMember(ClassType* obj, void(ClassType::* funcPtr)(), float timer,
	bool repeat) : m_pObject{ obj }, m_pFunction{ funcPtr }, m_Timer{.timer = timer, .repeat = repeat}
{

}

template <typename ClassType>
void TimerHandlerMember<ClassType>::HandleTimer()
{
	m_Timer.currentTimer += TimeEngine::GetInstance().GetDeltaTime();
	if(m_Timer.currentTimer >= m_Timer.timer)
	{
		(m_pObject->*m_pFunction)();
		if(m_Timer.repeat)
		{
			m_Timer.currentTimer = 0.f;
		}
	}
}

template <typename ClassType>
bool TimerHandlerMember<ClassType>::Equals(ITimerHandler* timer)
{
	if (auto castedEvent = dynamic_cast<TimerHandlerMember<ClassType>*>(timer))
	{
		return m_pObject == castedEvent->m_pObject && m_pFunction == castedEvent->m_pFunction && m_Timer.timer == m_Timer.timer;
	}
	return false;
}

template <typename ClassType>
bool TimerHandlerMember<ClassType>::IsDone() const
{
	return m_Timer.currentTimer >= m_Timer.timer;
}

template <typename ClassType>
void TimerManager::AddTimer(ClassType* obj, void(ClassType::* funcPtr)(), float timer, bool repeat)
{	
	m_TimerHandlers.emplace_back(std::make_unique<TimerHandlerMember<ClassType>>(obj, funcPtr, timer, repeat));
}

template <typename ClassType>
void TimerManager::RemoveTimer(ClassType* obj, void(ClassType::* funcPtr)(), float timer)
{
	m_TimerHandlers.erase(std::remove_if(m_TimerHandlers.begin(), m_TimerHandlers.end(), [obj, funcPtr, timer](const std::unique_ptr<ITimerHandler>& other)
		{
			return other->Equals(new TimerHandlerMember<ClassType>(obj, funcPtr, timer));
		}), m_TimerHandlers.end());
}

#pragma endregion structs

namespace Utils
{
#pragma region Functions

	bool IsPointInRectangle(const glm::vec2& point, const Rectf& rect);
	bool IsPointInRectangle(float x, float y, const Rectf& rect);

	bool IsPointInCircle(const glm::vec2& point, const Circlef& circle);
	bool IsPointInCircle(float x, float y, const Circlef& circle);

	bool LineIntersect2D(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& q0, const glm::vec2& q1, glm::vec2& intersectPoint);
	bool LineIntersect2D(const Linef& line1, const Linef& line2, glm::vec2& intersectPoint);

	bool IsOverlapping(const Rectf& rect1, const Rectf& rect2);

	bool Raycast2D(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, RaycastHitInfo2D& hitInfo);
	bool Raycast2D(const glm::vec2& origin, const glm::vec2& end, RaycastHitInfo2D& hitInfo);
	bool Raycast2D(const glm::vec2& origin, const glm::vec2& end, RaycastHitInfo2D& hitInfo, const std::string& tag);

	glm::vec2 GetCenterOfRectangle(const Rectf& rect);
	glm::vec2 GetPositionForRectangleToBeCentered(const Rectf& rect, const Rectf& otherRect);
#pragma endregion Functions
}