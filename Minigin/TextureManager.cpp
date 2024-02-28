#include "TextureManager.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

void TextureManager::Init()
{

}

void TextureManager::Update()
{
}

void TextureManager::SetTexture(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void TextureManager::Render() const
{
	const auto& pos = GetGameObject()->GetComponent<dae::Transform>()->GetPosition();
	dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void TextureManager::FixedUpdate()
{
}
