#include "SpriteComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "Utils.h"
#include <SDL_opengl.h>

#include "MathUtils.h"


SpriteComponent::SpriteComponent(const std::string& name, const std::string& filename): TextureComponent{name,filename}
{
	m_NrCols = 1;
	m_NrRows = 1;
	m_CurrentCol = 0;
	m_CurrentRow = 0;
	SetShape();
}

SpriteComponent::SpriteComponent(const std::string& name, const std::string& filename, unsigned nrCols, unsigned nrRows): SpriteComponent{name, filename}
{
	m_NrCols = nrCols;
	m_NrRows = nrRows;
	m_CurrentCol = 0;
	m_CurrentRow = 0;
	SetShape();
}

std::shared_ptr<BaseComponent> SpriteComponent::Clone() const
{
	return std::make_shared<SpriteComponent>(*this);
}

void SpriteComponent::Render() const
{
	glm::vec3 pos{};
	glm::vec3 rot{};
	if (const auto go{ GetGameObject() })
	{
		pos = go->GetWorldPosition();
		rot = go->GetWorldRotation();
	}
	pos += m_Offset.GetPosition();
	glPushMatrix();
	glTranslatef(pos.x,pos.y,0);

	if(MathUtils::Abs(rot.z) != 180.f)
	{
		glRotatef(rot.z, 0, 0, 1);
		glScalef(-1, 1, 1);
	}
	else
	{
		glRotatef(0, 0, 0, 1);
		glScalef(1, 1, 1);
	}
	glTranslatef(-m_Shape.width / 2, -m_Shape.height / 2,0);

	if (GetTexture() != nullptr)
	{
		dae::Renderer::GetInstance().RenderTexture(*GetTexture(),m_Shape,GetSrcRect());
	}
	glPopMatrix();
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

void SpriteComponent::SetCurrentRow(unsigned row)
{
	m_CurrentRow = row;
}

void SpriteComponent::SetCurrentCol(unsigned col)
{
	m_CurrentCol = col;
}

void SpriteComponent::SetCurrentFrame(unsigned frame)
{
	if(frame >= m_NrCols * m_NrRows || frame < 0)
	{
		frame = 0;
	}
	m_CurrentRow = frame / m_NrCols;
	m_CurrentCol = frame % m_NrCols;
}

void SpriteComponent::SetShape()
{
	m_Shape.topLeft.x = 0;
	m_Shape.topLeft.y = 0;
	m_Shape.width = static_cast<float>(GetSize().x) / static_cast<float>(m_NrCols);
	m_Shape.height = static_cast<float>(GetSize().y) / static_cast<float>(m_NrRows);
}

Rectf SpriteComponent::GetSrcRect() const
{
	Rectf srcRect{};
	srcRect.width = m_Shape.width ;
	srcRect.height = m_Shape.height ;
	srcRect.topLeft.x = static_cast<float>(m_CurrentCol % m_NrCols) * srcRect.width; // leave 0 for now
	srcRect.topLeft.y = srcRect.height * static_cast<float>(m_CurrentRow); // static_cast<float>(GetSize().y) + 0 * srcRect.height; // leave 0 for now
	return srcRect;
}
