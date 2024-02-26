#pragma once
#include <string>
#include <memory>

#include "BaseComponent.h"
#include "GameObject.h"
#include "Singleton.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextManager : public BaseComponent
	{
	public:
		TextManager(std::string text,const std::shared_ptr<Font>& font);
		void Init() override;
		void Update() override;
		void Render() const override;

		void SetFont(std::shared_ptr<Font> font);
		void SetText(const std::string& text);
		void SetPositionOffset(float x, float y);
		void FixedUpdate() override;

	private:
		bool m_needsUpdate{};
		std::string m_text;
		Transform m_Offset;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
