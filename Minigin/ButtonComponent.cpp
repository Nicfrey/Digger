#include "ButtonComponent.h"

#include "BoxCollider2D.h"
#include "TextComponent.h"


ButtonComponent::ButtonComponent(const std::string& name, const glm::vec3& pos, const std::string& textButton,
	const std::shared_ptr<dae::Font>& font, const std::string& fileNameTexture) : UIElementComponent{ name }, m_IsSelected{ false }
{
	m_Text = std::make_shared<dae::TextComponent>(textButton, font);
	m_Text->SetPositionOffset(pos.x, pos.y);
	m_Text->Update();
	if (!fileNameTexture.empty())
	{
		m_Image = std::make_shared<TextureComponent>(fileNameTexture);
		m_Image->SetPositionOffset(pos.x, pos.y);
	}
	m_Box = Rectf{ pos, static_cast<float>(m_Text->GetSize().x), static_cast<float>(m_Text->GetSize().y) };
}

ButtonComponent::ButtonComponent(const std::string& name, const glm::vec3& pos, const std::shared_ptr<dae::TextComponent>& text, const std::shared_ptr<TextureComponent>& image) : UIElementComponent(name), m_Text{ text }, m_Image{ image }, m_IsSelected{ false }
{
	m_Box = Rectf{ pos, static_cast<float>(m_Text->GetSize().x), static_cast<float>(m_Text->GetSize().y) };
}

void ButtonComponent::SetOnButtonClick(const std::function<void()>& func)
{
	m_OnButtonClickFunc = func;
}

void ButtonComponent::OnClick(const glm::vec2& posMouse) const
{
	if(!Utils::IsPointInRectangle(posMouse,m_Box))
	{
		return;
	}

	if (m_OnButtonClickFunc)
	{
		m_OnButtonClickFunc();
	}
}

void ButtonComponent::OnPressed() const
{
	if (!m_IsSelected)
	{
		return;
	}
	if (m_OnButtonClickFunc && m_IsSelected)
	{
		m_OnButtonClickFunc();
	}
}

void ButtonComponent::RenderElement() const
{
	if (m_Image)
	{
		m_Image->RenderElement();
	}
	m_Text->RenderElement();
}

void ButtonComponent::SetTextButton(const std::string& text)
{
	m_Text->SetText(text);
	m_Text->Update();
	m_Box = Rectf{ m_Box.topLeft, static_cast<float>(m_Text->GetSize().x), static_cast<float>(m_Text->GetSize().y) };
}
