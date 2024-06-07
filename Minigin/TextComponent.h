#pragma once
#include <string>
#include <memory>

#include "TextureComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent : public TextureComponent
	{
	public:
		TextComponent(std::string text,const std::shared_ptr<Font>& font);
		~TextComponent() override = default;
		TextComponent(const TextComponent& other) = default;
		TextComponent(TextComponent&& other) noexcept;
		TextComponent& operator=(const TextComponent& other);
		TextComponent& operator=(TextComponent&& other) noexcept;
		std::shared_ptr<BaseComponent> Clone() const override;
		void Update() override;
		void Render() const override;
		void RenderElement() const override;
		void UpdateElement() override;

		void SetFont(const std::shared_ptr<Font>& font);
		void SetText(const std::string& text);
		std::string GetText();
		std::shared_ptr<Font>& GetFont();
		void SetColor(int r, int g, int b, int a = 255);

	private:
		void UpdateText();
		bool m_needsUpdate{};
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		glm::ivec4 m_Color{255,255,255,255};
	};
}
