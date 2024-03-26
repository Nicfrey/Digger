#pragma once
#include <glm/vec2.hpp>

#pragma region structs

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

#pragma endregion structs

#pragma region Functions


#pragma endregion Functions

