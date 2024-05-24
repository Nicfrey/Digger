#pragma once
#include <functional>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Singleton.h"

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
	};

	using DelegateFnc = std::function<void()>;

	class TimerManager final : public dae::Singleton<TimerManager>
	{
	public:
		template<typename ClassType>
		void AddTimer(ClassType* obj, void (ClassType::* funcPtr)(),float timer);
		void AddTimer(const DelegateFnc& function, float timer);
		void Update();
	private:
		struct TimerHandler
		{
			Timer timer;
			DelegateFnc func;
		};
		std::vector<TimerHandler> m_TimerHandlers;
	};

template <typename ClassType>
void TimerManager::AddTimer(ClassType* obj, void(ClassType::* funcPtr)(), float timer)
{
	Timer newTimer{ .timer = timer };
	TimerHandler newHandler{ .timer = newTimer, .func = [obj, funcPtr]()
	{
		(obj->*funcPtr)();
	} };
	m_TimerHandlers.emplace_back(newHandler);
}

#pragma endregion structs

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
#pragma endregion Functions

