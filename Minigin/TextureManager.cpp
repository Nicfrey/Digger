#include "TextureManager.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

void TextureManager::Init()
{
	m_PositionRender = glm::vec3{};
}

void TextureManager::Update()
{
}

void TextureManager::SetTexture(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void TextureManager::UpdatePosition(const glm::vec3& newPosition)
{
	m_PositionRender = newPosition;
}

void TextureManager::Render() const
{
	const auto& pos = m_PositionRender;
	dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}
