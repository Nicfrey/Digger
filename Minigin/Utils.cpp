#include "Utils.h"

glm::vec2 Rectf::GetCenter() const
{
	return { bottomLeft.x + width / 2,bottomLeft.y + height / 2 };
}
