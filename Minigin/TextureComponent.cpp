#include "TextureComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

TextureComponent::TextureComponent(const std::string& filename): BaseComponent{nullptr}, m_Offset{}
{
	SetTexture(filename);
}

void TextureComponent::Init()
{
}

void TextureComponent::Update()
{
}

void TextureComponent::SetTexture(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void TextureComponent::SetTexture(const std::shared_ptr<dae::Texture2D>& texture)
{
	m_Texture = texture;
}

void TextureComponent::SetPositionOffset(float x, float y)
{
	m_Offset.SetPosition(x, y, 0);
}

void TextureComponent::Render() const
{
	if(m_Texture != nullptr)
	{
		glm::vec3 pos{};
		if(const auto go{GetGameObject()})
		{
			pos = go->GetWorldPosition();
		}
		pos += m_Offset.GetPosition();
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void TextureComponent::FixedUpdate()
{
}

void TextureComponent::RenderGUI()
{
}

glm::ivec2 TextureComponent::GetSize() const
{
	return m_Texture->GetSize();
}

std::shared_ptr<dae::Texture2D> TextureComponent::GetTexture() const
{
	return m_Texture;
}
