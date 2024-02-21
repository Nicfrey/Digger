#pragma once
#include "GameObjectComponent.h"

class FpsComponent final : GameObjectComponent
{
public:
	FpsComponent() = default;
	~FpsComponent() override;
	void Update() override;
	void Init() override;
	void Render() const override;
	float GetFps() const;

private:
	float m_Fps;
};

