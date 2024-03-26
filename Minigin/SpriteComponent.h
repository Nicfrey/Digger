#pragma once
#include "TextureComponent.h"
#include "Utils.h"

class SpriteComponent final : public TextureComponent
{
public:
	SpriteComponent() = default;
	SpriteComponent(const std::string& filename);
	SpriteComponent(const std::string& filename, unsigned nrCols, unsigned nrRows);
	~SpriteComponent() override = default;

	SpriteComponent(const SpriteComponent& other) = delete;
	SpriteComponent(SpriteComponent&& other) noexcept = delete;
	SpriteComponent& operator=(const SpriteComponent& other) = delete;
	SpriteComponent& operator=(SpriteComponent&& other) noexcept = delete;
	void Init() override;
	void Update() override;
	void Render() const override;
	void FixedUpdate() override;
	void RenderGUI() override;
	Rectf GetShape() const;
	void SetNrCols(unsigned nrCols);
	void SetNrRows(unsigned nrRows);
private:
	Rectf GetSrcRect() const;
	void SetShape();

	Rectf m_Shape;
	float m_Scale{ 1.f };

	unsigned m_NrCols;
	unsigned m_NrRows;
};

