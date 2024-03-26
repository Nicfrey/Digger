#include "SpriteComponent.h"

#include "GameObject.h"
#include "Renderer.h"

SpriteComponent::SpriteComponent(const std::string& filename): TextureComponent{filename}
{
	SetShape();
}

SpriteComponent::SpriteComponent(const std::string& filename, unsigned nrCols, unsigned nrRows): SpriteComponent{filename}
{
	m_NrCols = nrCols;
	m_NrRows = nrRows;
	SetShape();
}

void SpriteComponent::Init()
{
	TextureComponent::Init();
}

void SpriteComponent::Update()
{
	glm::vec3 pos{};
	if (const auto go{ GetGameObject() })
	{
		pos = go->GetWorldPosition();
	}
	pos += m_Offset.GetPosition();
	m_Shape.bottomLeft.x = pos.x;
	m_Shape.bottomLeft.y = pos.y;
}

void SpriteComponent::Render() const
{
	if (GetTexture() != nullptr)
	{
		dae::Renderer::GetInstance().RenderTexture(*GetTexture(),m_Shape,GetSrcRect());
	}
}

void SpriteComponent::FixedUpdate()
{
	TextureComponent::FixedUpdate();
}

void SpriteComponent::RenderGUI()
{
	TextureComponent::RenderGUI();
}

Rectf SpriteComponent::GetShape() const
{
	return m_Shape;
}

void SpriteComponent::SetNrCols(unsigned nrCols)
{
	m_NrCols = nrCols;
}

void SpriteComponent::SetNrRows(unsigned nrRows)
{
	m_NrRows = nrRows;
}

void SpriteComponent::SetShape()
{
	m_Shape.bottomLeft.x = 0;
	m_Shape.bottomLeft.y = 0;
	m_Shape.width = static_cast<float>(GetSize().x) / static_cast<float>(m_NrCols);
	m_Shape.height = static_cast<float>(GetSize().y) / static_cast<float>(m_NrRows);
}

Rectf SpriteComponent::GetSrcRect() const
{
	Rectf srcRect{};
	srcRect.width = m_Shape.width ;
	srcRect.height = m_Shape.height ;
	srcRect.bottomLeft.x = 0 * srcRect.width; // leave 0 for now
	srcRect.bottomLeft.y = 0; // static_cast<float>(GetSize().y) + 0 * srcRect.height; // leave 0 for now
	return srcRect;
}
