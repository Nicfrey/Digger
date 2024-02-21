#include "FpsCounterComponent.h"

#include "Time.h"

FpsCounterComponent::FpsCounterComponent(const std::shared_ptr<dae::Font>& font): TextManager{std::to_string(Time::GetFps()),font}
{
}

void FpsCounterComponent::Init()
{
	TextManager::Init();
}

void FpsCounterComponent::Update()
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(m_Precision) << Time::GetFps();
	SetText(ss.str());
	TextManager::Update();
}

void FpsCounterComponent::Render() const
{
	TextManager::Render();
}

void FpsCounterComponent::SetPrecision(int precision)
{
	m_Precision = precision;
}
