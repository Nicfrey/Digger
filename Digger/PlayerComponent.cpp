#include "PlayerComponent.h"

#include "AnimatorComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ProjectileComponent.h"
#include "SceneManager.h"
#include "BoxCollider2D.h"
#include "imgui.h"
#include "Observer.h"
#include "SpriteComponent.h"
#include "Scene.h"

PlayerComponent::PlayerComponent() : BaseComponent(nullptr), m_pProjectile{ nullptr }
{
	EventManager::GetInstance().AddEvent("PlayerDied", this, &PlayerComponent::SetDeadAnim);
}

PlayerComponent::PlayerComponent(const PlayerComponent& other): BaseComponent(other)
{
	m_pProjectile = new dae::GameObject(*other.m_pProjectile);
	m_pDefaultProjectile = std::make_unique<dae::GameObject>(*other.m_pDefaultProjectile);
}

PlayerComponent::PlayerComponent(PlayerComponent&& other) noexcept : BaseComponent(std::move(other))
{
	m_pProjectile = std::move(other.m_pProjectile);
	m_pDefaultProjectile = std::move(other.m_pDefaultProjectile);
	other.m_pProjectile = nullptr;
}

PlayerComponent& PlayerComponent::operator=(const PlayerComponent& other)
{
	if(this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(other);
	delete m_pProjectile;
	m_pProjectile = nullptr;
	m_pProjectile = new dae::GameObject(*other.m_pProjectile);
	m_pDefaultProjectile = std::make_unique<dae::GameObject>(*other.m_pDefaultProjectile);
	return *this;
}

PlayerComponent& PlayerComponent::operator=(PlayerComponent&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(std::move(other));
	delete m_pProjectile;
	m_pProjectile = nullptr;
	m_pProjectile = std::move(other.m_pProjectile);
	m_pDefaultProjectile = std::move(other.m_pDefaultProjectile);
	return *this;
}

std::shared_ptr<BaseComponent> PlayerComponent::Clone() const
{
	return std::make_shared<PlayerComponent>(*this);
}

void PlayerComponent::Update()
{
	BaseComponent::Update();
}

void PlayerComponent::Init()
{
	BaseComponent::Init();
	if(GetGameObject()->HasComponent<AnimatorComponent>())
	{
		const auto animator{ GetGameObject()->GetComponent<AnimatorComponent>() };
		animator->AddParameter("HasProjectile", true);
		animator->AddParameter("PlayerDied", false);
	}
	
}

void PlayerComponent::RenderGUI()
{
	BaseComponent::RenderGUI();
	ImGui::Begin("Player");
	ImGui::Text("Player Projectile :");
	ImGui::Indent();
	if(m_pProjectile)
		ImGui::Text("Position : %f %f", m_pProjectile->GetWorldPosition().x, m_pProjectile->GetWorldPosition().y);
	ImGui::End();
}

void PlayerComponent::OnCollisionEnter(std::shared_ptr<dae::GameObject>& other)
{
	HandleCollisionProjectile(other);
}

void PlayerComponent::ShootProjectile()
{
	if(!m_pProjectile)
	{
		const auto newProjectile{ CreateProjectile() };
		m_pProjectile = newProjectile.get();
		dae::SceneManager::GetInstance().Instantiate(newProjectile);
		if(GetGameObject()->HasComponent<AnimatorComponent>())
		{
			auto animator = GetGameObject()->GetComponent<AnimatorComponent>();
			animator->SetParameter("HasProjectile", false);
		}
	}
}

void PlayerComponent::ResetProjectile()
{
	m_pProjectile = nullptr;
	if (GetGameObject()->HasComponent<AnimatorComponent>())
	{
		auto animator = GetGameObject()->GetComponent<AnimatorComponent>();
		animator->SetParameter("HasProjectile", true);
	}
}

void PlayerComponent::HandleCollisionProjectile(std::shared_ptr<dae::GameObject>& other) const
{
	if(other->IsDestroyed())
	{
		return;
	}

	if (const auto projectileOther{ other->GetComponent<ProjectileComponent>() })
	{
		if (GetGameObject() != projectileOther->GetShotBy())
		{
			const auto health{ GetGameObject()->GetComponent<HealthComponent>() };
			health->LoseOneLife();
			const auto playerComp{ projectileOther->GetShotBy()->GetComponent<PlayerComponent>() };
			playerComp->ResetProjectile();
			other->Destroy();
		}
	}
}

std::shared_ptr<dae::GameObject> PlayerComponent::CreateProjectile() const
{
	const auto forward{ GetGameObject()->GetForward() };
	const std::shared_ptr newProjectile{ std::make_shared<dae::GameObject>()};
	const auto projectileComponent{ std::make_shared<ProjectileComponent>(forward) };
	newProjectile->AddComponent(projectileComponent);
	const auto sprite = std::make_shared<SpriteComponent>("SpritesFire.png", 3, 2);
	newProjectile->AddComponent(std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height));
	newProjectile->AddComponent(sprite);
	newProjectile->GetComponent<ProjectileComponent>()->Activate(GetGameObject());
	newProjectile->SetLocalPosition(GetGameObject()->GetWorldPosition().x + 5 * forward.x, GetGameObject()->GetWorldPosition().y + 5 * forward.y);
	return newProjectile;
}

void PlayerComponent::SetDeadAnim()
{
	if(GetGameObject()->HasComponent<HealthComponent>())
	{
		const auto health{ GetGameObject()->GetComponent<HealthComponent>() };
		if (GetGameObject()->HasComponent<AnimatorComponent>() && health->IsDead())
		{
			const auto animator = GetGameObject()->GetComponent<AnimatorComponent>();
			animator->SetParameter("PlayerDied", true);
		}
	}
}
