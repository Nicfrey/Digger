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
		void Init() override;
		void Update() override;
		void Render() const override;

		void SetFont(const std::shared_ptr<Font>& font);
		void SetText(const std::string& text);
		void FixedUpdate() override;
		void RenderGUI() override;

	private:
		bool m_needsUpdate{};
		std::string m_text;
		std::shared_ptr<Font> m_Font;
	};
}
