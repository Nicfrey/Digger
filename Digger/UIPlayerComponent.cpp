#include "UIPlayerComponent.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Observer.h"
#include "Widget.h"
#include "WidgetManager.h"

std::shared_ptr<BaseComponent> UIPlayerComponent::Clone() const
{
	return std::make_shared<UIPlayerComponent>(*this);
}

void UIPlayerComponent::Init()
{
	m_WidgetUI = WidgetManager::GetInstance().GetWidget("GameUI");
	EventManager::GetInstance().AddEvent("ScoreAdded",this,&UIPlayerComponent::UpdateScore);
	EventManager::GetInstance().AddEvent("LifeLost", this, &UIPlayerComponent::UpdateLives);
	UpdateLives();
	UpdateScore();
}

void UIPlayerComponent::OnDestroy()
{
	EventManager::GetInstance().RemoveEvent("ScoreAdded", this, &UIPlayerComponent::UpdateScore);
	EventManager::GetInstance().RemoveEvent("LifeLost", this, &UIPlayerComponent::UpdateLives);
}

void UIPlayerComponent::UpdateScore()
{
	int score;
	GameInstance::GetInstance().GetValue("Score",score);
	m_WidgetUI->GetElement<dae::TextComponent>("Score")->SetText(std::to_string(score));
}

void UIPlayerComponent::UpdateLives()
{
	int lives;
	GameInstance::GetInstance().GetValue("Lives", lives);
	m_WidgetUI->GetElement<dae::TextComponent>("Life")->SetText(std::to_string(lives));
}
