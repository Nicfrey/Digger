#pragma once
#include <memory>
#include <string>

#include "BaseComponent.h"
#include "Transform.h"

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
	void SetPositionOffset(float x, float y);
	void Render() const override;
	void FixedUpdate() override;
	void RenderGUI() override;
	glm::ivec2 GetSize() const;
	std::shared_ptr<dae::Texture2D> GetTexture() const;

protected:
	dae::Transform m_Offset;
private:
	std::shared_ptr<dae::Texture2D> m_Texture{};
};

