#pragma once
#include <memory>
#include <string>

#include "BaseComponent.h"
#include "Transform.h"
#include "UIElementComponent.h"

namespace dae
{
	class Texture2D;
}

class TextureComponent : public UIElementComponent, public BaseComponent
{
public:
	TextureComponent(const std::string& name);
	TextureComponent(const std::string& name, const std::string& filename);
	void SetTexture(const std::string& filename);
	void SetTexture(const std::shared_ptr<dae::Texture2D>& texture);
	void SetPositionOffset(float x, float y);
	void Render() const override;
	glm::ivec2 GetSize() const;
	std::shared_ptr<dae::Texture2D> GetTexture() const;
	std::shared_ptr<BaseComponent> Clone() const override;

protected:
	dae::Transform m_Offset;
private:
	std::shared_ptr<dae::Texture2D> m_Texture{};
};

