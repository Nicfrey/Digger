#include "BoxCollider2D.h"
#include "GameObject.h"
#include "imgui.h"
#include "Utils.h"

BoxCollider2D::BoxCollider2D(): Collider2D{}
{
	if(GetGameObject())
	{
		m_BoxCollider.topLeft.x = GetGameObject()->GetWorldPosition().x;
		m_BoxCollider.topLeft.y = GetGameObject()->GetWorldPosition().y;
	}
}

BoxCollider2D::BoxCollider2D(float width, float height): BoxCollider2D{}
{
	m_BoxCollider.width = width;
	m_BoxCollider.height = height;
}

BoxCollider2D::BoxCollider2D(const BoxCollider2D& other) : Collider2D{other}
{
	m_BoxCollider = other.m_BoxCollider;
}

BoxCollider2D::BoxCollider2D(BoxCollider2D&& other) noexcept: Collider2D{std::move(other)}
{
	m_BoxCollider = std::move(other.m_BoxCollider);
	other.m_BoxCollider = Rectf{};
}

BoxCollider2D& BoxCollider2D::operator=(const BoxCollider2D& other)
{
	if(this == &other)
	{
		return *this;
	}
	Collider2D::operator=(other);
	m_BoxCollider = other.m_BoxCollider;
	return *this;
}

BoxCollider2D& BoxCollider2D::operator=(BoxCollider2D&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}
	Collider2D::operator=(std::move(other));
	m_BoxCollider = std::move(other.m_BoxCollider);
	other.m_BoxCollider = Rectf{};
	return *this;
}

void BoxCollider2D::Init()
{
	Collider2D::Init();
	if (GetGameObject())
	{
		m_BoxCollider.topLeft.x = GetGameObject()->GetWorldPosition().x;
		m_BoxCollider.topLeft.y = GetGameObject()->GetWorldPosition().y;
	}
}

void BoxCollider2D::RenderGUI()
{
	const ImVec2 TopLeft{m_BoxCollider.topLeft.x , m_BoxCollider.topLeft.y};
	const ImVec2 bottomRight{ m_BoxCollider.topLeft.x + m_BoxCollider.width,m_BoxCollider.topLeft.y + m_BoxCollider.height };
	ImGui::Begin("Collisions BoxCollider2D");
	ImGui::GetWindowDrawList()->AddRect(TopLeft, bottomRight, IM_COL32(0, 255, 0, 255));
	ImGui::End();
}

std::shared_ptr<BaseComponent> BoxCollider2D::Clone() const
{
	return std::make_shared<BoxCollider2D>(*this);
}

void BoxCollider2D::Update()
{
	if(GetGameObject())
	{
		m_BoxCollider.topLeft.x = GetGameObject()->GetWorldPosition().x;
		m_BoxCollider.topLeft.y = GetGameObject()->GetWorldPosition().y;
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
		if (m_BoxCollider.topLeft.x + m_BoxCollider.width < otherBox->GetBoxCollider().topLeft.x || (otherBox->GetBoxCollider().topLeft.x + otherBox->GetBoxCollider().width) < m_BoxCollider.topLeft.x)
		{
			if(GetOther())
			{
				GetGameObject()->OnCollisionExit(other);
			}
			SetOther(nullptr);
			return false;
		}

		// If one rectangle is under the other
		if (m_BoxCollider.topLeft.y + m_BoxCollider.height < otherBox->GetBoxCollider().topLeft.y || (otherBox->GetBoxCollider().topLeft.y + otherBox->GetBoxCollider().height) < m_BoxCollider.topLeft.y)
		{
			if (GetOther())
			{
				GetGameObject()->OnCollisionExit(other);
			}
			SetOther(nullptr);
			return false;
		}
		if (GetOther())
		{
			GetGameObject()->OnCollisionStay(other);
		}
		else
		{
			GetGameObject()->OnCollisionEnter(other);
			SetOther(other.get());
		}
		return true;
	}
	return false;
}

bool BoxCollider2D::Intersect(const glm::vec3& p0, const glm::vec3& p1, glm::vec3& intersection, dae::GameObject* go) const
{
	// Regroup every point of the box
	std::vector<glm::vec2> points;
	points.emplace_back(m_BoxCollider.topLeft);
	points.emplace_back(m_BoxCollider.topLeft + glm::vec2{ m_BoxCollider.width,0 });
	points.emplace_back(m_BoxCollider.topLeft + glm::vec2{ 0,m_BoxCollider.height });
	points.emplace_back(m_BoxCollider.topLeft + glm::vec2{ m_BoxCollider.width , m_BoxCollider.height });

	for(size_t i{}; i < points.size(); ++i)
	{
		const size_t j{ (i + 1) % points.size() };
		glm::vec2 intersectPoint;
		if(Utils::LineIntersect2D(p0,p1,points[i],points[j],intersectPoint))
		{
			go = GetGameObject();
			intersection = glm::vec3{ intersectPoint.x,intersectPoint.y,0 };
			return true;
		}
	}
	return false;
}

bool BoxCollider2D::IsRaycasting(std::shared_ptr<dae::GameObject>& other)
{
	if(Collider2D::IsRaycasting(other))
	{
		return false;
	}
	// Intersect with one line
	if(const auto otherBox{other->GetComponent<BoxCollider2D>()})
	{
		// TODO
	}
	return false;
}

bool BoxCollider2D::IsColliding(std::shared_ptr<dae::GameObject>& other)
{
	if(Collider2D::IsColliding(other))
	{
		return false;
	}
	bool collide{ false };
	if(const auto otherBox{other->GetComponent<BoxCollider2D>()})
	{
		if (Utils::IsOverlapping(otherBox->GetBoxCollider(),GetBoxCollider()))
		{
			const float overlapX{ std::min(GetGameObject()->GetWorldPosition().x + GetBoxCollider().width - other->GetWorldPosition().x, other->GetWorldPosition().x + otherBox->GetBoxCollider().width - GetGameObject()->GetWorldPosition().x) };
			const float overlapY{ std::min(GetGameObject()->GetWorldPosition().y + GetBoxCollider().height - other->GetWorldPosition().y, other->GetWorldPosition().y + otherBox->GetBoxCollider().height - GetGameObject()->GetWorldPosition().y) };
			if(overlapX < overlapY)
			{
				if(GetGameObject()->GetWorldPosition().x < other->GetWorldPosition().x)
				{
					GetGameObject()->Translate(-overlapX / 2,0);
					other->Translate(overlapX / 2,0);
					collide = true;
				}
				else
				{
					GetGameObject()->Translate(overlapX / 2,0);
					other->Translate(-overlapX / 2,0);
					collide = true;
				}
			}
			else
			{
				if(GetGameObject()->GetWorldPosition().y < other->GetWorldPosition().y)
				{
					GetGameObject()->Translate(0,-overlapY / 2);
					other->Translate(0,overlapY / 2);
					collide = true;
				}
				else
				{
					GetGameObject()->Translate(0,overlapY / 2);
					other->Translate(0,-overlapY / 2);
					collide = true;
				}
			}
		}
	}
	return collide;
}

Rectf BoxCollider2D::GetBoxCollider() const
{
	return m_BoxCollider;
}

void BoxCollider2D::SetSize(const Rectf& size)
{
	m_BoxCollider = size;
}

void BoxCollider2D::SetWidth(float width)
{
	m_BoxCollider.width = width;
}

void BoxCollider2D::SetHeight(float height)
{
	m_BoxCollider.height = height;
}
