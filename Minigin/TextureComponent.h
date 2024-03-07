#pragma once
#include <memory>
#include <string>
#include <glm/vec3.hpp>

#include "BaseComponent.h"
#include "Singleton.h"

namespace dae
{
	class Texture2D;
}

class TextureComponent : public BaseComponent
{
public:
	TextureComponent() = default;
	TextureComponent(const std::string& filename);
	void Init() override;
	void Update() override;
	void SetTexture(const std::string& filename);
	void SetTexture(const std::shared_ptr<dae::Texture2D>& texture);
	void Render() const override;
	void FixedUpdate() override;
	void RenderGUI() override;

private:
	std::shared_ptr<dae::Texture2D> m_Texture{};
};

