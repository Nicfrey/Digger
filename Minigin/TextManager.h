#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Singleton.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextManager final : public Singleton<TextManager>
	{
	public:
		void Init();
		void Update();
		void Render() const;

		void SetFont(std::shared_ptr<Font> font);
		void SetText(const std::string& text);
		void SetPosition(float x, float y);
	private:
		bool m_needsUpdate{};
		std::string m_text;
		Transform m_transform{};
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
