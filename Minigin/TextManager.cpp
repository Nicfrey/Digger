#include <stdexcept>
#include <SDL_ttf.h>
#include "TextManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextManager::TextManager(std::string text, const std::shared_ptr<Font>& font):
	m_needsUpdate{true}, m_text{std::move(text)}, m_font{font}, m_textTexture{nullptr}, m_Offset{}
{
}

void dae::TextManager::Init()
{
	m_textTexture = nullptr;
	m_font = nullptr;
	m_needsUpdate = true;
	m_text = "";
}

void dae::TextManager::Update()
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextManager::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = GetGameObject()->GetTransform().GetPosition() + m_Offset.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

void dae::TextManager::SetFont(std::shared_ptr<Font> font)
{

}

// This implementation uses the "dirty flag" pattern
void dae::TextManager::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextManager::SetPositionOffset(const float x, const float y)
{
	m_Offset.SetPosition(x, y, 0.0f);
}

void dae::TextManager::FixedUpdate()
{
}


