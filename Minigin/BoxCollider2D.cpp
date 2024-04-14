#include "BoxCollider2D.h"

#include "GameObject.h"
BoxCollider2D::BoxCollider2D(): Collider2D{}
{
	if(GetGameObject())
	{
		m_BoxCollider.bottomLeft.x = GetGameObject()->GetWorldPosition().x;
		m_BoxCollider.bottomLeft.y = GetGameObject()->GetWorldPosition().y;
	}
}

BoxCollider2D::BoxCollider2D(float width, float height): BoxCollider2D{}
{
	m_BoxCollider.width = width;
	m_BoxCollider.height = height;
}

void BoxCollider2D::Update()
{
	if(GetGameObject())
	{
		m_BoxCollider.bottomLeft.x = GetGameObject()->GetLocalPosition().x;
		m_BoxCollider.bottomLeft.y = GetGameObject()->GetLocalPosition().y;
	}
}

bool BoxCollider2D::IsOverlapping(std::shared_ptr<dae::GameObject>& other)
{
	if(!Collider2D::IsOverlapping(other))
	{
		return false;
	}

	ColliderComponent* otherCollider{ (other->GetComponent<ColliderComponent>().get()) };

	if (const auto otherBox{ dynamic_cast<BoxCollider2D*>(otherCollider) })
	{
		// If one rectangle is on left side of the other
		if (m_BoxCollider.bottomLeft.x + m_BoxCollider.width < otherBox->GetBoxCollider().bottomLeft.x || (otherBox->GetBoxCollider().bottomLeft.x + otherBox->GetBoxCollider().width) < m_BoxCollider.bottomLeft.x)
		{
			SetOther(nullptr);
			// TODO call OnCollisionExit
			GetGameObject()->OnCollisionExit(other);
			return false;
		}

		// If one rectangle is under the other
		if (m_BoxCollider.bottomLeft.y + m_BoxCollider.height < otherBox->GetBoxCollider().bottomLeft.y || (otherBox->GetBoxCollider().bottomLeft.y + otherBox->GetBoxCollider().height) < m_BoxCollider.bottomLeft.y)
		{
			SetOther(nullptr);
			// TODO call OnCollisionExit
			GetGameObject()->OnCollisionExit(other);
			return false;
		}
		if (GetOther())
		{
			// TODO call OnCollisionStay
			GetGameObject()->OnCollisionStay(other);
		}
		else
		{
			SetOther(other.get());
			// TODO call OnCollisionEnter
			GetGameObject()->OnCollisionEnter(other);
		}
		return true;
	}

	// TODO Circle

	return false;
}

Rectf BoxCollider2D::GetBoxCollider() const
{
	return m_BoxCollider;
}

void BoxCollider2D::SetSize(const Rectf& size)
{
	m_BoxCollider = size;
}
