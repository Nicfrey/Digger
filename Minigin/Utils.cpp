#include "Utils.h"

#include <iostream>

#include "Collider2D.h"
#include "GameObject.h"
#include "MathUtils.h"
#include "SceneManager.h"
#include "TimeEngine.h"

glm::vec2 Rectf::GetCenter() const
{
	return { bottomLeft.x + width / 2,bottomLeft.y + height / 2 };
}

void TimerManager::AddTimer(const DelegateFnc& function, float timer)
{
	Timer newTimer{ .timer = timer };
	TimerHandler newTimerHandler{ newTimer, function };
	m_TimerHandlers.emplace_back(newTimerHandler);
}


void TimerManager::Update()
{
	for (auto& handler : m_TimerHandlers)
	{
		handler.timer.currentTimer += TimeEngine::GetInstance().GetDeltaTime();
		if(handler.timer.currentTimer >= handler.timer.timer)
		{
			handler.func();
		}
	}
	std::erase_if(m_TimerHandlers, [&](const TimerHandler& other)
	{
		return other.timer.currentTimer >= other.timer.timer;
	});
}

bool IsPointInRectangle(const glm::vec2& point, const Rectf& rect)
{
	return point.x >= rect.bottomLeft.x && point.x <= rect.bottomLeft.x + rect.width &&
		point.y >= rect.bottomLeft.y && point.y <= rect.bottomLeft.y + rect.height;
}

bool IsPointInRectangle(float x, float y, const Rectf& rect)
{
	return IsPointInRectangle(glm::vec2(x, y),rect);
}

bool IsPointInCircle(const glm::vec2& point, const Circlef& circle)
{
	return glm::distance(point, circle.center) <= circle.radius;
}

bool IsPointInCircle(float x, float y, const Circlef& circle)
{
	return IsPointInCircle(glm::vec2(x, y), circle);
}

bool LineIntersect2D(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& q0, const glm::vec2& q1,
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

bool LineIntersect2D(const Linef& line1, const Linef& line2, glm::vec2& intersectPoint)
{
	return LineIntersect2D(line1.p0, line1.p1, line2.p0, line2.p1, intersectPoint);
}

bool IsOverlapping(const Rectf& rect1, const Rectf& rect2)
{
	return rect1.bottomLeft.x < rect2.bottomLeft.x + rect2.width &&
		rect1.bottomLeft.x + rect1.width > rect2.bottomLeft.x &&
		rect1.bottomLeft.y < rect2.bottomLeft.y + rect2.height &&
		rect1.bottomLeft.y + rect1.height > rect2.bottomLeft.y;
}

bool Raycast2D(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, RaycastHitInfo2D& hitInfo)
{
	const glm::vec2 end = origin + direction * maxDistance;
	return Raycast2D(origin, end, hitInfo);
}

bool Raycast2D(const glm::vec2& origin, const glm::vec2& end, RaycastHitInfo2D& hitInfo)
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

bool Raycast2D(const glm::vec2& origin, const glm::vec2& end, RaycastHitInfo2D& hitInfo, const std::string& tag)
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
