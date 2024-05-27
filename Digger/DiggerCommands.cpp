#include "DiggerCommands.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "MathUtils.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "TimeEngine.h"

MoveCommand::MoveCommand(dae::GameObject* go, glm::vec2 direction): GameObjectCommand{go}
{
	m_Direction = normalize(direction);
}

void MoveCommand::Execute()
{
	const glm::vec2 velocity{ m_Direction * TimeEngine::GetInstance().GetDeltaTime() * m_Speed };
	GetGameObject()->Translate(velocity);
	GetGameObject()->SetLocalRotation(0, 0, MathUtils::Rad2Deg(MathUtils::Atan2(m_Direction.y, m_Direction.x)));
	EventManager::GetInstance().NotifyEvent("PlayerMoving");
}

ShootCommand::ShootCommand(dae::GameObject* go): GameObjectCommand{ go }
{
}

void ShootCommand::Execute()
{
	if(const auto playerComp{GetGameObject()->GetComponent<PlayerComponent>()})
	{
		playerComp->ShootProjectile();
	}
}
