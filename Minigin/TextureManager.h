#pragma once
#include <memory>
#include <string>
#include <glm/vec3.hpp>

#include "GameObjectComponent.h"
#include "Singleton.h"

namespace dae
{
	class Texture2D;
}

class TextureManager final: public GameObjectComponent
{
public:
	void Init() override;
	void Update() override;
	void SetTexture(const std::string& filename);
	void UpdatePosition(const glm::vec3& newPosition);
	void Render() const override;

private:
	glm::vec3 m_PositionRender{};
	std::shared_ptr<dae::Texture2D> m_Texture{};
};

