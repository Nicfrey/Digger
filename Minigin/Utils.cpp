#include "Utils.h"

#include <iostream>

#include "Collider2D.h"
#include "GameObject.h"
#include "MathUtils.h"
#include "SceneManager.h"
#include "TimeEngine.h"

glm::vec2 Rectf::GetCenter() const
{
	return { topLeft.x + width / 2,topLeft.y + height / 2 };
}

TimerHandlerFunction::TimerHandlerFunction(const DelegateFnc& func, float timer, bool repeat): m_Handler{func}, m_Timer{.timer = timer, .repeat = repeat}
{
}

void TimerHandlerFunction::HandleTimer()
{
	m_Timer.currentTimer += TimeEngine::GetInstance().GetDeltaTime();
	if(m_Timer.currentTimer >= m_Timer.timer)
	{
		m_Handler();
		if(m_Timer.repeat)
		{
			m_Timer.currentTimer = 0.f;
		}
	}
}

bool TimerHandlerFunction::Equals(ITimerHandler* timer)
{
	if (auto* p = dynamic_cast<TimerHandlerFunction*>(timer))
	{
		return m_Handler.target<void()>() == p->m_Handler.target<void()>();
	}
	return false;
}

bool TimerHandlerFunction::IsDone() const
{
	return m_Timer.currentTimer >= m_Timer.timer;
}

void TimerManager::AddTimer(const DelegateFnc& function, float timer, bool repeat)
{
	m_TimerHandlers.emplace_back(std::make_unique<TimerHandlerFunction>(function, timer, repeat));
}

void TimerManager::Update()
{
	for(const auto& timer: m_TimerHandlers)
	{
		timer->HandleTimer();
	}
	RemoveTimerDone();
}

void TimerManager::RemoveTimer(const DelegateFnc& function, float timer)
{
	std::erase_if(m_TimerHandlers, [function, timer](const std::unique_ptr<ITimerHandler>& other)
	{
		return other->Equals(new TimerHandlerFunction(function, timer));
	});
}

void TimerManager::RemoveTimerDone()
{
	std::erase_if(m_TimerHandlers, [](const std::unique_ptr<ITimerHandler>& other)
		{
			return other->IsDone();
		});
}

bool Utils::IsPointInRectangle(const glm::vec2& point, const Rectf& rect)
{
	return point.x >= rect.topLeft.x && point.x <= rect.topLeft.x + rect.width &&
		point.y >= rect.topLeft.y && point.y <= rect.topLeft.y + rect.height;
}

bool Utils::IsPointInRectangle(float x, float y, const Rectf& rect)
{
	return IsPointInRectangle(glm::vec2(x, y),rect);
}

bool Utils::IsPointInCircle(const glm::vec2& point, const Circlef& circle)
{
	return glm::distance(point, circle.center) <= circle.radius;
}

bool Utils::IsPointInCircle(float x, float y, const Circlef& circle)
{
	return IsPointInCircle(glm::vec2(x, y), circle);
}

bool Utils::LineIntersect2D(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& q0, const glm::vec2& q1,
                     glm::vec2& intersectPoint)
{
	const glm::vec2 u1{ p1 - p0 };
	const glm::vec2 u2{ q1 - q0 };
	const glm::vec2 v{ q0 - p0 };
	const float d{ MathUtils::Cross2D(u1,u2) };
	const float d1{ MathUtils::Cross2D(v,u2) };
	const float d2{ MathUtils::Cross2D(v,u1) };
	const float t1{ d1 / d };
	const float t2{ d2 / d };
	if((t1 >= 0 && t1 <= 1) && (t2 >= 0 && t2 <= 0))
	{
		intersectPoint = p0 + t1 * u1;
		return true;
	}
	return false;
}

bool Utils::LineIntersect2D(const Linef& line1, const Linef& line2, glm::vec2& intersectPoint)
{
	return LineIntersect2D(line1.p0, line1.p1, line2.p0, line2.p1, intersectPoint);
}

bool Utils::IsOverlapping(const Rectf& rect1, const Rectf& rect2)
{
	return rect1.topLeft.x < rect2.topLeft.x + rect2.width &&
		rect1.topLeft.x + rect1.width > rect2.topLeft.x &&
		rect1.topLeft.y < rect2.topLeft.y + rect2.height &&
		rect1.topLeft.y + rect1.height > rect2.topLeft.y;
}

bool Utils::Raycast2D(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, RaycastHitInfo2D& hitInfo)
{
	const glm::vec2 end = origin + direction * maxDistance;
	return Utils::Raycast2D(origin, end, hitInfo);
}

bool Utils::Raycast2D(const glm::vec2& origin, const glm::vec2& end, RaycastHitInfo2D& hitInfo)
{
	const auto gameObjects{ dae::SceneManager::GetInstance().GetAllGameObject() };
	for (const auto& gameObject : gameObjects)
	{
		if(gameObject->HasComponent<Collider2D>())
		{
			const auto collider{ gameObject->GetComponent<Collider2D>() };

			glm::vec3 intersectPoint{};
			dae::GameObject* go{nullptr};
			if (collider->Intersect(glm::vec3{ origin.x, origin.y,0 }, glm::vec3{end.x,end.y,0}, intersectPoint,go))
			{
				hitInfo.point = { intersectPoint.x,intersectPoint.y };
				hitInfo.pGameObject = go;
				hitInfo.distance = glm::distance(origin, hitInfo.point);
				return true;
			} 
		}
	}
	return false;
}

bool Utils::Raycast2D(const glm::vec2& origin, const glm::vec2& end, RaycastHitInfo2D& hitInfo, const std::string& tag)
{
	const auto gameObjects{ dae::SceneManager::GetInstance().GetGameObjectsByTag(tag) };
	for (const auto& gameObject : gameObjects)
	{
		if (gameObject->HasComponent<Collider2D>())
		{
			const auto collider{ gameObject->GetComponent<Collider2D>() };

			glm::vec3 intersectPoint{};
			dae::GameObject* go{ nullptr };
			if (collider->Intersect(glm::vec3{ origin.x, origin.y,0 }, glm::vec3{ end.x,end.y,0 }, intersectPoint, go))
			{
				hitInfo.point = { intersectPoint.x,intersectPoint.y };
				hitInfo.pGameObject = go;
				hitInfo.distance = glm::distance(origin, hitInfo.point);
				return true;
			}
		}
	}
	return false;
}

glm::vec2 Utils::GetCenterOfRectangle(const Rectf& rect)
{
	return { rect.topLeft.x + rect.width / 2,rect.topLeft.y + rect.height / 2 };
}

glm::vec2 Utils::GetPositionForRectangleToBeCentered(const Rectf& rect, const Rectf& otherRect)
{
	return { otherRect.topLeft.x + otherRect.width / 2 - rect.width / 2, otherRect.topLeft.y + otherRect.height / 2 - rect.height / 2 };
}
