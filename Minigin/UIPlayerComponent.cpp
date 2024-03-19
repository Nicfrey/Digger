#include "UIPlayerComponent.h"

#include "HealthComponent.h"
#include "Observer.h"

UIPlayerComponent::UIPlayerComponent(const std::shared_ptr<dae::Font>& font): BaseComponent{ nullptr }, m_pLifeText{new dae::TextComponent{"#lives: ",font}}, m_pScoreText{new dae::TextComponent{"#score: ",font}}
{
}

UIPlayerComponent::~UIPlayerComponent()
{
	delete m_pLifeText;
	delete m_pScoreText;
	m_pLifeText = nullptr;
	m_pScoreText = nullptr;
}

void UIPlayerComponent::Init()
{
	EventManager::GetInstance().AddEvent("LifeGained", [this]() { UpdateTextLife(); });
	EventManager::GetInstance().AddEvent("LifeLost", [this]() { UpdateTextLife(); });

	UpdateTextLife();
}

void UIPlayerComponent::Update()
{
	m_pLifeText->Update();
}

void UIPlayerComponent::Render() const
{
	m_pLifeText->Render();
}

void UIPlayerComponent::FixedUpdate()
{
}

void UIPlayerComponent::RenderGUI()
{
}

void UIPlayerComponent::SetPositionTextLife(float x, float y) const
{
	m_pLifeText->SetPositionOffset(x, y);
}

void UIPlayerComponent::SetPositionTextLife(const glm::vec3& pos) const
{
	SetPositionTextLife(pos.x, pos.y);
}

void UIPlayerComponent::SetPositionTextScore(const glm::vec3& pos) const
{
	SetPositionTextScore(pos.x, pos.y);
}

void UIPlayerComponent::SetPositionTextScore(float x, float y) const
{
	m_pScoreText->SetPositionOffset(x, y);
}

void UIPlayerComponent::UpdateTextLife() const
{
	if(const auto healthComponent{ GetGameObject()->GetComponent<HealthComponent>() })
	{
		const std::string newText{ "#life: " + std::to_string(healthComponent->GetLifeRemaining()) };
		m_pLifeText->SetText(newText);
	}
}
