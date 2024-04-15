#pragma once
#include "TextureComponent.h"
#include "Utils.h"

class SpriteComponent final : public TextureComponent
{
public:
	SpriteComponent() = default;
	SpriteComponent(const std::string& filename);
	SpriteComponent(const std::string& filename, unsigned nrCols, unsigned nrRows);
	std::shared_ptr<BaseComponent> Clone() const override;
	void Render() const override;
	Rectf GetShape() const;
	void SetNrCols(unsigned nrCols);
	void SetNrRows(unsigned nrRows);
	void SetCurrentRow(unsigned row);
	void SetCurrentCol(unsigned col);
private:
	Rectf GetSrcRect() const;
	void SetShape();

	Rectf m_Shape;
	float m_Scale{ 1.f };

	unsigned m_NrCols;
	unsigned m_NrRows;
	unsigned m_CurrentRow;
	unsigned m_CurrentCol;
};

