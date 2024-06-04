#include "PlayerComponent.h"

#include <iostream>

#include "AnimatorComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ProjectileComponent.h"
#include "SceneManager.h"
#include "BoxCollider2D.h"
#include "DiggerUtils.h"
#include "EnemyComponent.h"
#include "imgui.h"
#include "NavMeshAgentComponent.h"
#include "Observer.h"
#include "SpriteComponent.h"
#include "Scene.h"
#include "SoundSystemEngine.h"

PlayerComponent::PlayerComponent() : BaseComponent(nullptr), m_pProjectile{ nullptr }
{
	EventManager::GetInstance().AddEvent("PlayerDied", this, &PlayerComponent::HandleDeath);
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
	if(!m_NavMeshComp->HasReachedDestination())
	{
		EventManager::GetInstance().NotifyEvent("PlayerMoving");
	}
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
	if(GetGameObject()->HasComponent<NavMeshAgentComponent>())
	{
		m_NavMeshComp = GetGameObject()->GetComponent<NavMeshAgentComponent>();
	}
}

void PlayerComponent::HandleCollisionEnemy(const std::shared_ptr<dae::GameObject>& other) const
{
	const auto health = GetGameObject()->GetComponent<HealthComponent>();
	if(health->IsDead())
	{
		return;
	}
	if(other->HasComponent<EnemyComponent>())
	{
		health->LoseOneLife();
		if(health->IsDead())
		{
			EventManager::GetInstance().NotifyEvent("PlayerDied");
		}
	}
}

void PlayerComponent::OnCollisionEnter(std::shared_ptr<dae::GameObject>& other)
{
	HandleCollisionProjectile(other);
	HandleCollisionEnemy(other);
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
		EventManager::GetInstance().AddEvent("ProjectileHit", this, &PlayerComponent::ProjectileHasCollide);
	}
}

void PlayerComponent::ProjectileHasCollide()
{
	if(m_pProjectile->IsDestroyed())
	{
		TimerManager::GetInstance().AddTimer(this, &PlayerComponent::ResetProjectile, 5.f);
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

void PlayerComponent::HandleCollisionProjectile(std::shared_ptr<dae::GameObject>& other)
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
			other->Destroy();
			EventManager::GetInstance().NotifyEvent("ProjectileHit");
		}
	}
}

std::shared_ptr<dae::GameObject> PlayerComponent::CreateProjectile() const
{
	const auto forward{ GetGameObject()->GetForward() };
	const std::shared_ptr newProjectile{ std::make_shared<dae::GameObject>()};
	const auto projectileComponent{ std::make_shared<ProjectileComponent>(forward) };
	newProjectile->AddComponent(projectileComponent);
	const auto sprite = std::make_shared<SpriteComponent>("Projectile", "SpritesFire.png", 3, 2);
	const auto boxCollider = std::make_shared<BoxCollider2D>(sprite->GetShape().width, sprite->GetShape().height);
	const auto animator = std::make_shared<AnimatorComponent>();
	const Animation idle{ .name = "Idle", .frames = {0,1,2}, .frameTime = 0.3f,.loop = true, .spriteComponent = sprite };
	animator->AddAnimation(idle);
	if(animator->SetStartAnimation(idle))
	{
		std::cerr << "Failed to start animation of Projectile object\n";
	}
	boxCollider->SetIsStatic(false);
	newProjectile->AddComponent(boxCollider);
	newProjectile->AddComponent(sprite);
	newProjectile->AddComponent(animator);
	newProjectile->GetComponent<ProjectileComponent>()->Activate(GetGameObject());
	newProjectile->SetLocalPosition(GetGameObject()->GetWorldPosition().x + 5 * forward.x, GetGameObject()->GetWorldPosition().y + 5 * forward.y);
	return newProjectile;
}

void PlayerComponent::HandleDeath()
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
	ServiceMusicLocator::GetMusicSystem().Play(static_cast<MusicId>(DiggerUtils::MusicDiggerID::GAME_OVER),false);
	EventManager::GetInstance().NotifyEvent("RestartLevel");
}