#include "FpsComponent.h"

#include "TimeEngine.h"

FpsComponent::~FpsComponent() = default;

void FpsComponent::Update()
{
	m_Fps = 1.f / TimeEngine::GetInstance().GetDeltaTime();
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
