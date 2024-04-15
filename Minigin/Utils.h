#pragma once
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#pragma region structs

namespace dae
{
	class GameObject;
}

	struct Rectf final
	{
		glm::vec2 bottomLeft;
		float width;
		float height;
		glm::vec2 GetCenter() const;
	};

	struct Circlef final
	{
		glm::vec2 center;
		float radius;
	};

	struct RaycastHitInfo2D
	{
		glm::vec2 point;
		glm::vec2 normal;
		float distance;
		dae::GameObject* pGameObject;
	};

#pragma endregion structs

#pragma region Functions

	bool LineIntersect2D(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& q0, const glm::vec2& q1, glm::vec2& intersectPoint);

	bool Raycast2D(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, RaycastHitInfo2D& hitInfo);
	bool Raycast2D(const glm::vec2& origin, const glm::vec2& end, RaycastHitInfo2D& hitInfo);
	bool Raycast2D(const glm::vec2& origin, const glm::vec2& end, RaycastHitInfo2D& hitInfo, const std::string& tag);
#pragma endregion Functions

