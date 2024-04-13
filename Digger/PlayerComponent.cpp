#include "PlayerComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "ProjectileComponent.h"
#include "SceneManager.h"
#include "BoxCollider2D.h"
#include "imgui_plot.h"
#include "SpriteComponent.h"
#include "Scene.h"

PlayerComponent::PlayerComponent() : BaseComponent(nullptr), m_pProjectile{ nullptr }
{
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
		std::shared_ptr newProjectile{ std::make_shared<dae::GameObject>() };
		newProjectile = std::make_shared<dae::GameObject>();
		newProjectile->AddComponent(std::make_shared<ProjectileComponent>());
		const auto sprite = std::make_shared<SpriteComponent>("SpritesFire.png", 3, 2);
		newProjectile->AddComponent(std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height));
		newProjectile->AddComponent(sprite);
		dae::SceneManager::GetInstance().Instantiate(newProjectile);
		newProjectile->GetComponent<ProjectileComponent>()->Activate();
		newProjectile->SetLocalPosition(GetGameObject()->GetWorldPosition().x + 5, GetGameObject()->GetWorldPosition().y);
		m_pProjectile = newProjectile.get();
	}
}

void PlayerComponent::HandleCollisionProjectile(std::shared_ptr<dae::GameObject>& other) const
{
	if (const auto projectileOther{ other->GetComponent<ProjectileComponent>() })
	{
		const auto myProjectile = m_pProjectile->GetComponent<ProjectileComponent>();
		if (projectileOther != myProjectile)
		{
			const auto health{ GetGameObject()->GetComponent<HealthComponent>() };
			health->LoseOneLife();
			dae::SceneManager::GetInstance().Destroy(other);
		}
	}
}
