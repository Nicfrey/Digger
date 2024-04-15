#include "FpsComponent.h"
#include "TextComponent.h"
#include "TimeEngine.h"

#include <iomanip>
#include <ios>
#include <sstream>


FpsComponent::FpsComponent(const FpsComponent& other): BaseComponent{other}
{
	m_pTextComponent = other.m_pTextComponent;
	m_Fps = other.m_Fps;
}

FpsComponent::FpsComponent(FpsComponent&& other) noexcept: BaseComponent(std::move(other))
{
	m_pTextComponent = std::move(other.m_pTextComponent);
	m_Fps = std::move(other.m_Fps);
	other.m_Fps = 0.f;
}

FpsComponent& FpsComponent::operator=(const FpsComponent& other)
{
	if (this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(other);
	m_pTextComponent = other.m_pTextComponent;
	m_Fps = other.m_Fps;

	return *this;
}

FpsComponent& FpsComponent::operator=(FpsComponent&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(std::move(other));
	m_pTextComponent = std::move(other.m_pTextComponent);
	m_Fps = std::move(other.m_Fps);
	other.m_Fps = 0.f;

	return *this;
}

void FpsComponent::Update()
{
	m_Fps = 1.f / TimeEngine::GetInstance().GetDeltaTime();
	std::stringstream ss;
	ss << std::fixed << std::setprecision(m_Precision) << m_Fps;
	m_pTextComponent->SetText(ss.str());
	m_pTextComponent->Update();
}

void FpsComponent::Render() const
{
	m_pTextComponent->Render();
}

float FpsComponent::GetFps() const
{
	return m_Fps;
}

std::shared_ptr<BaseComponent> FpsComponent::Clone() const
{
	return std::make_shared<FpsComponent>(*this);
}
