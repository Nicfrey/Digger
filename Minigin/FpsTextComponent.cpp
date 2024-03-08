#include "FpsTextComponent.h"
#include "FpsComponent.h"
#include "Time.h"

FpsTextComponent::FpsTextComponent(const std::shared_ptr<dae::Font>& font) : TextComponent{ "",font }, m_FpsComponent{ std::make_shared<FpsComponent>() }
{

}

void FpsTextComponent::Init()
{
	TextComponent::Init();
	m_FpsComponent->Init();
}

void FpsTextComponent::Update()
{
	m_FpsComponent->Update();
	std::stringstream ss;
	ss << std::fixed << std::setprecision(m_Precision) << m_FpsComponent->GetFps();
	SetText(ss.str());
	TextComponent::Update();
}

void FpsTextComponent::Render() const
{
	TextComponent::Render();
}

void FpsTextComponent::SetPrecision(int precision)
{
	m_Precision = precision;
}

void FpsTextComponent::FixedUpdate()
{
}

void FpsTextComponent::RenderGUI()
{
}
