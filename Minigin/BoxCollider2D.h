#pragma once
#include "BaseComponent.h"
#include "Collider2D.h"
#include "Utils.h"


class BoxCollider2D : public Collider2D
{
public:
	BoxCollider2D();
	BoxCollider2D(float width, float height);
	BoxCollider2D(const BoxCollider2D& other);
	BoxCollider2D(BoxCollider2D&& other) noexcept;
	BoxCollider2D& operator=(const BoxCollider2D& other);
	BoxCollider2D& operator=(BoxCollider2D&& other) noexcept;
	void Init() override;
	void RenderGUI() override;
	std::shared_ptr<BaseComponent> Clone() const override;

	void Update() override;
	bool IsOverlapping(std::shared_ptr<dae::GameObject>& other) override;
	bool Intersect(const glm::vec3& p0, const glm::vec3& p1, glm::vec3& intersection, dae::GameObject*) const override;
	bool IsRaycasting(std::shared_ptr<dae::GameObject>& other) override;

	Rectf GetBoxCollider() const;
	void SetSize(const Rectf& size);

private:
	// Overlap and raycast
	Rectf m_BoxCollider;

	// Raycast
	Linef m_VerticalLine;
	Linef m_HorizontalLine;
};

