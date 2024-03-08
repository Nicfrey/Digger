#include "FpsComponent.h"

#include "Time.h"

FpsComponent::~FpsComponent() = default;

void FpsComponent::Update()
{
	m_Fps = 1.f / Time::GetDeltaTime();
}

void FpsComponent::Init()
{
}

void FpsComponent::Render() const
{
}

float FpsComponent::GetFps() const
{
	return m_Fps;
}

void FpsComponent::FixedUpdate()
{
}

void FpsComponent::RenderGUI()
{
}
