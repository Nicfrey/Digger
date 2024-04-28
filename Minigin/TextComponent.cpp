#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(std::string text, const std::shared_ptr<Font>& font) : TextureComponent{},
	m_needsUpdate{true}, m_Text{std::move(text)}, m_Font{font}
{
	
}

dae::TextComponent::TextComponent(TextComponent&& other) noexcept: TextureComponent(std::move(other))
{
	m_Text = std::move(other.m_Text);
	m_Font = std::move(other.m_Font);
	m_needsUpdate = other.m_needsUpdate;
}

dae::TextComponent& dae::TextComponent::operator=(const TextComponent& other)
{
	if (this == &other)
	{
		return *this;
	}
	TextureComponent::operator=(other);
	m_Text = other.m_Text;
	m_Font = other.m_Font;
	m_needsUpdate = other.m_needsUpdate;
	return *this;
}

dae::TextComponent& dae::TextComponent::operator=(TextComponent&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	TextureComponent::operator=(std::move(other));
	m_Text = std::move(other.m_Text);
	m_Font = std::move(other.m_Font);
	m_needsUpdate = other.m_needsUpdate;
	return *this;
}

std::shared_ptr<BaseComponent> dae::TextComponent::Clone() const
{
	return std::make_shared<TextComponent>(*this);
}

void dae::TextComponent::Update()
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
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
		SetTexture(std::make_shared<Texture2D>(texture));
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	TextureComponent::Render();
}

void dae::TextComponent::SetFont(const std::shared_ptr<Font>& font)
{
	m_Font = font;
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::FixedUpdate()
{
}

void dae::TextComponent::RenderGUI()
{
}

std::string dae::TextComponent::GetText()
{
	return m_Text;
}

std::shared_ptr<dae::Font>& dae::TextComponent::GetFont()
{
	return m_Font;
}


