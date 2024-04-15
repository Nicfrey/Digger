#include "UIPlayerComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "Observer.h"
#include "ScoreComponent.h"

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

UIPlayerComponent::UIPlayerComponent(const UIPlayerComponent& other): BaseComponent{ other }, m_pLifeText{ new dae::TextComponent{*other.m_pLifeText} }, m_pScoreText{ new dae::TextComponent{*other.m_pScoreText} }
{
}

UIPlayerComponent::UIPlayerComponent(UIPlayerComponent&& other) noexcept: BaseComponent{ std::move(other) }
{
	m_pLifeText = other.m_pLifeText;
	m_pScoreText = other.m_pScoreText;
	other.m_pLifeText = nullptr;
	other.m_pScoreText = nullptr;
}

UIPlayerComponent& UIPlayerComponent::operator=(const UIPlayerComponent& other)
{
	if (this != &other)
	{
		BaseComponent::operator=(other);
		delete m_pLifeText;
		m_pLifeText = nullptr;
		delete m_pScoreText;
		m_pScoreText = nullptr;
		m_pLifeText = new dae::TextComponent{*other.m_pLifeText};
		m_pScoreText = new dae::TextComponent{*other.m_pScoreText};
	}
	return *this;
}

UIPlayerComponent& UIPlayerComponent::operator=(UIPlayerComponent&& other) noexcept
{
	if (this != &other)
	{
		BaseComponent::operator=(std::move(other));
		delete m_pLifeText;
		m_pLifeText = nullptr;
		delete m_pScoreText;
		m_pScoreText = nullptr;
		m_pLifeText = std::move(other.m_pLifeText);
		m_pScoreText = std::move(other.m_pScoreText);
		other.m_pLifeText = nullptr;
		other.m_pScoreText = nullptr;
	}
	return *this;
}

std::shared_ptr<BaseComponent> UIPlayerComponent::Clone() const
{
	return std::make_shared<UIPlayerComponent>(*this);
}

void UIPlayerComponent::Init()
{
	EventManager::GetInstance().AddEvent("LifeGained", [this]() { UpdateTextLife(); });
	EventManager::GetInstance().AddEvent("LifeLost", [this]() { UpdateTextLife(); });
	EventManager::GetInstance().AddEvent("ScoreAdded", [this]() {UpdateTextScore(); });

	UpdateTextLife();
	UpdateTextScore();
}

void UIPlayerComponent::Update()
{
	m_pLifeText->Update();
	m_pScoreText->Update();
}

void UIPlayerComponent::Render() const
{
	m_pLifeText->Render();
	m_pScoreText->Render();
}

void UIPlayerComponent::FixedUpdate()
{
}

void UIPlayerComponent::RenderGUI()
{
}

void UIPlayerComponent::SetPosition(float x, float y) const
{
	SetPositionTextLife(x, y);
	SetPositionTextScore(x, y + 20);
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
		std::string newText{};
		if (healthComponent->IsDead())
		{
			newText = "Player Is Dead";
		}
		else
		{
			newText = "#life: " + std::to_string(healthComponent->GetLifeRemaining());
		}
		m_pLifeText->SetText(newText);
	}
}

void UIPlayerComponent::UpdateTextScore() const
{
	if (const auto scoreComponent{ GetGameObject()->GetComponent<ScoreComponent>() })
	{
		const std::string newText{ "#score: " + std::to_string(scoreComponent->GetScore()) };
		m_pScoreText->SetText(newText);
	}
}
