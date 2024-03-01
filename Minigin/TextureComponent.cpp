#include "TextureComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

TextureComponent::TextureComponent(const std::string& filename): BaseComponent{nullptr}
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

void TextureComponent::Render() const
{
	if(m_Texture != nullptr)
	{
		const glm::vec3 pos{GetGameObject()->GetWorldPosition()};
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void TextureComponent::FixedUpdate()
{
}
