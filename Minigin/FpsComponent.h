#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TextComponent;
}

class FpsComponent final : public BaseComponent
{
public:
	FpsComponent() = default;
	~FpsComponent() override = default;
	FpsComponent(const FpsComponent& other);
	FpsComponent(FpsComponent&& other) noexcept;
	FpsComponent& operator=(const FpsComponent& other);
	FpsComponent& operator=(FpsComponent&& other) noexcept;
	void Update() override;
	void Render() const override;
	float GetFps() const;
	std::shared_ptr<BaseComponent> Clone() const override;
private:
	int m_Precision{ 1 };
	float m_Fps;
	std::shared_ptr<dae::TextComponent> m_pTextComponent;
};

