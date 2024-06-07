#pragma once
#include <string>
#include <vector>

#include "BaseComponent.h"
#include "Blackboard.h"
#include "FiniteStateMachine.h"

class SpriteComponent;

class AnimationController : public FiniteStateMachine
{
public:
	AnimationController(Blackboard* pBlackboard);
	AnimationController(FSMStateNode* pStartState, Blackboard* pBlackboard);
	~AnimationController() override = default;
	AnimationController(const AnimationController& other) = default;
	AnimationController(AnimationController&& other) noexcept;
	AnimationController& operator=(const AnimationController& other);
	AnimationController& operator=(AnimationController&& other) noexcept;
	FSMStateNode* GetStateNode(const std::string& name) const;
};

struct Animation
{
	std::string name;
	std::vector<int> frames;
	float frameTime = 0.3f;
	size_t currentFrame = 0;
	float timer = 0.f;
	bool loop = false;
	std::shared_ptr<SpriteComponent> spriteComponent;
};

class AnimationStateNode final : public FSMStateNode
{
public:
	AnimationStateNode(const Animation& anim) : FSMStateNode{} { m_CurrentAnimation = anim; }
	~AnimationStateNode() override = default;
	void Enter(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard) override;
	void Exit(Blackboard* pBlackboard) override;
	std::string GetName() const { return m_CurrentAnimation.name; }
	bool IsFinished() const { return m_IsFinished; }
private:
	Animation m_CurrentAnimation;
	bool m_IsFinished{ false };
};

class SpriteComponent;
/**
 * \brief Component used to create animation for a GameObject
 * Need to have a sprite component to work
 */
class AnimatorComponent: public BaseComponent
{
public:
	AnimatorComponent();
	~AnimatorComponent() override;
	AnimatorComponent(const AnimatorComponent& other);
	AnimatorComponent(AnimatorComponent&& other) noexcept;
	AnimatorComponent& operator=(const AnimatorComponent& other);
	AnimatorComponent& operator=(AnimatorComponent&& other) noexcept;
	std::shared_ptr<BaseComponent> Clone() const override;
	void Init() override;
	void Update() override;

	bool CurrentAnimationIsFinished() const;
	void AddAnimation(const Animation& anim) const;
	bool SetStartAnimation(const Animation& anim) const;
	void AddTransition(const Animation& startAnim, const Animation& endAnim, FSMTransition* pTransition) const;
	template<typename T>
	void AddParameter(const std::string& key, T value);
	template<typename T>
	void SetParameter(const std::string& key, T value);
private:
	std::shared_ptr<SpriteComponent> m_pSpriteComponent{ nullptr };
	AnimationController* m_pAnimationController;
};

template <typename T>
void AnimatorComponent::AddParameter(const std::string& key, T value)
{
	m_pAnimationController->GetBlackboard()->AddValue(key, value);
}

template <typename T>
void AnimatorComponent::SetParameter(const std::string& key, T value)
{
	m_pAnimationController->GetBlackboard()->ChangeValue(key, value);
}


