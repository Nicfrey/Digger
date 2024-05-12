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

		void SetFont(const std::shared_ptr<Font>& font);
		void SetText(const std::string& text);
		void FixedUpdate() override;
		void RenderGUI() override;
		std::string GetText();
		std::shared_ptr<Font>& GetFont();

	private:
		bool m_needsUpdate{};
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
	};
}
