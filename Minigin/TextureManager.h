#pragma once
#include <memory>
#include <string>
#include <glm/vec3.hpp>

#include "Singleton.h"

namespace dae
{
	class Texture2D;
}

class TextureManager final: public dae::Singleton<TextureManager>
{
public:
	void Init();
	void SetTexture(const std::string& filename);
	void UpdatePosition(const glm::vec3& newPosition);
	void Render() const;

private:
	glm::vec3 m_PositionRender{};
	std::shared_ptr<dae::Texture2D> m_Texture{};
};

