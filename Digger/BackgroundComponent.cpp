#include "BackgroundComponent.h"

#include "GameObject.h"
#include "Graph.h"
 #include "LevelComponent.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "ProjectileComponent.h"
#include "Scene.h"
#include "SceneManager.h"

std::shared_ptr<BaseComponent> BackgroundComponent::Clone() const
{
	return std::make_shared<BackgroundComponent>(*this);
}

void BackgroundComponent::OnCollisionEnter(std::shared_ptr<dae::GameObject>& other)
{
	if(other->HasComponent<PlayerComponent>())
	{
		GetGameObject()->Destroy();
	}
	if(other->HasComponent<ProjectileComponent>())
	{
		const auto projectile{ other->GetComponent<ProjectileComponent>() };
		const auto playerComp{ projectile->GetShotBy()->GetComponent<PlayerComponent>() };
		other->Destroy();
		EventManager::GetInstance().NotifyEvent("ProjectileHit");
	}
}

void BackgroundComponent::Init()
{
}

void BackgroundComponent::Update()
{
	// TODO Not doing that every frame but at some moment of time and with a thread
	/*
	const auto goLevelComponent{ dae::SceneManager::GetInstance().GetGameObjectWithComponent<LevelComponent>() };
	const auto levelComp{ goLevelComponent->GetComponent<LevelComponent>() };
	const auto closestNode = levelComp->GetGraph()->GetClosestNode(GetGameObject()->GetWorldPosition());
	{
		if(closestNode->CanBeVisited())
		{
			GetGameObject()->Destroy();
		}
	}
	*/
}

void BackgroundComponent::HandleDestruction(const GraphUtils::Graph* pLevel)
{
	if(!GetGameObject())
	{
		return;
	}
	const auto closestNode{ pLevel->GetClosestNode(GetGameObject()->GetWorldPosition()) };
	if (closestNode->CanBeVisited())
	{
		GetGameObject()->Destroy();
	}
}
