#pragma once
#include <glm/glm.hpp>

#include "BaseComponent.h"

namespace dae
{
	class Transform final
	{
	public:
		Transform() = default;
		Transform(const glm::vec3& pos);

		const glm::vec3& GetLocalPosition() const { return m_Position; }
		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalPosition(const glm::vec2& pos);
		const glm::vec3& GetRotation() const { return m_Rotation; }
		void SetRotation(float x, float y, float z);
		void SetRotation(float x, float y);
		const glm::vec3& GetScale() const { return m_Scale; }
		void SetScale(float x, float y, float z);
		void SetScale(float scale);

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};
}
