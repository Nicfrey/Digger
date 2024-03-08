#pragma once
#include "BaseComponent.h"

class FpsComponent final : BaseComponent
{
public:
	FpsComponent() = default;
	~FpsComponent() override;
	void Update() override;
	void Init() override;
	void Render() const override;
	float GetFps() const;
	void FixedUpdate() override;
	void RenderGUI() override;

private:
	float m_Fps;
};

