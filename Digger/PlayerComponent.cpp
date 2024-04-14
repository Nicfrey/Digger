#include "PlayerComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "ProjectileComponent.h"
#include "SceneManager.h"
#include "BoxCollider2D.h"
#include "imgui.h"
#include "SpriteComponent.h"
#include "Scene.h"

PlayerComponent::PlayerComponent() : BaseComponent(nullptr), m_pProjectile{ nullptr }
{
}

void PlayerComponent::Init()
{
	BaseComponent::Init();
}

void PlayerComponent::Update()
{
	BaseComponent::Update();
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
		auto forward{ GetGameObject()->GetForward() };
		std::shared_ptr newProjectile{ std::make_shared<dae::GameObject>() };
		newProjectile = std::make_shared<dae::GameObject>();
		const auto projectileComponent{ std::make_shared<ProjectileComponent>( forward ) };
		newProjectile->AddComponent(projectileComponent);
		const auto sprite = std::make_shared<SpriteComponent>("SpritesFire.png", 3, 2);
		newProjectile->AddComponent(std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height));
		newProjectile->AddComponent(sprite);
		dae::SceneManager::GetInstance().Instantiate(newProjectile);
		newProjectile->GetComponent<ProjectileComponent>()->Activate(GetGameObject());
		newProjectile->SetLocalPosition(GetGameObject()->GetWorldPosition().x + 5 * forward.x, GetGameObject()->GetWorldPosition().y + 5 * forward.y);
		m_pProjectile = newProjectile.get();
	}
}

void PlayerComponent::ResetProjectile()
{
	m_pProjectile = nullptr;
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
