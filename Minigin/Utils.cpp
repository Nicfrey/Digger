#include "Utils.h"

#include "Collider2D.h"
#include "GameObject.h"
#include "MathUtils.h"
#include "SceneManager.h"

glm::vec2 Rectf::GetCenter() const
{
	return { bottomLeft.x + width / 2,bottomLeft.y + height / 2 };
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
