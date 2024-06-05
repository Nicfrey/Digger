#pragma once
#include "TextComponent.h"

class UIPlayerComponent : public BaseComponent
{
public:
	UIPlayerComponent(const std::shared_ptr<dae::Font>& font);
	~UIPlayerComponent() override;
	UIPlayerComponent(const UIPlayerComponent& other);
	UIPlayerComponent(UIPlayerComponent&& other) noexcept;
	UIPlayerComponent& operator=(const UIPlayerComponent& other);
	UIPlayerComponent& operator=(UIPlayerComponent&& other) noexcept;
	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
	void Update() override;
	void Render() const override;
	void FixedUpdate() override;
	void RenderGUI() override;
	void OnDestroy() override;

	void SetPosition(float x, float y) const;
private:
	void UpdateTextLife();
	void UpdateTextScore();
	void SetPositionTextLife(float x, float y) const;
	void SetPositionTextLife(const glm::vec3& pos) const;
	void SetPositionTextScore(const glm::vec3& pos) const;
	void SetPositionTextScore(float x, float y) const;
	dae::TextComponent* m_pLifeText;
	dae::TextComponent* m_pScoreText;
};

