#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		Transform() = default;
		Transform(const glm::vec3& pos);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(float x, float y);
		void SetPosition(const glm::vec3& pos);
		void SetPosition(const glm::vec2& pos);
		const glm::vec3& GetRotation() const { return m_Rotation; }
		void SetRotation(float x, float y, float z);
		void SetRotation(float x, float y);
		void SetRotation(const glm::vec3& rot);
		void SetRotation(const glm::vec2& rot);
		const glm::vec3& GetScale() const { return m_Scale; }
		void SetScale(float x, float y, float z);
		void SetScale(float scale);
		glm::vec3 GetForward() const;

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};
}
