#include "AnimatorComponent.h"

#include <iostream>

#include "Blackboard.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "TimeEngine.h"

AnimationController::AnimationController(Blackboard* pBlackboard): FiniteStateMachine{pBlackboard}
{
}

AnimationController::AnimationController(FSMStateNode* pStartState, Blackboard* pBlackboard) : FiniteStateMachine{ pStartState,pBlackboard }
{
}

AnimationController::AnimationController(AnimationController&& other) noexcept: FiniteStateMachine{std::move(other)}
{
}

AnimationController& AnimationController::operator=(const AnimationController& other)
{
	if(this == &other)
	{
		return *this;
	}
	FiniteStateMachine::operator=(other);
	return *this;
}

AnimationController& AnimationController::operator=(AnimationController&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}
	FiniteStateMachine::operator=(other);
	return *this;
}

FSMStateNode* AnimationController::GetStateNode(const std::string& name) const
{
	for(const auto transition : m_Transitions)
	{
		auto nodeAnim = dynamic_cast<AnimationStateNode*>(transition.first);
		if(nodeAnim && nodeAnim->GetName() == name)
		{
			return nodeAnim;
		}
		for(const auto transitionAnim : transition.second)
		{
			nodeAnim = dynamic_cast<AnimationStateNode*>(transitionAnim.second);
			if(nodeAnim && nodeAnim->GetName() == name)
			{
				return nodeAnim;
			}
		}
	}
	return nullptr;
}

void AnimationStateNode::Enter(Blackboard* pBlackboard)
{
	if(!pBlackboard || m_CurrentAnimation.spriteComponent == nullptr)
	{
		std::cerr << "AnimationStateNode::Enter() pBlackboard or spriteComponent is nullptr\n";
		return;
	}
	m_CurrentAnimation.currentFrame = 0;
	m_CurrentAnimation.timer = 0.f;
	m_CurrentAnimation.spriteComponent->SetCurrentFrame(m_CurrentAnimation.frames[m_CurrentAnimation.currentFrame]);
	m_IsFinished = false;
}

void AnimationStateNode::Update(Blackboard* pBlackboard)
{
	if (!pBlackboard || m_CurrentAnimation.spriteComponent == nullptr)
	{
		std::cerr << "AnimationStateNode::Update() pBlackboard or spriteComponent is nullptr\n";
		return;
	}
	m_CurrentAnimation.timer += TimeEngine::GetInstance().GetDeltaTime();
	if(m_CurrentAnimation.timer >= m_CurrentAnimation.frameTime)
	{
		m_CurrentAnimation.timer -= m_CurrentAnimation.frameTime;
		m_CurrentAnimation.currentFrame++;
		if(m_CurrentAnimation.currentFrame >= m_CurrentAnimation.frames.size())
		{
			m_CurrentAnimation.currentFrame = 0;
			if(!m_CurrentAnimation.loop)
			{
				m_IsFinished = true;
			}
		}
		m_CurrentAnimation.spriteComponent->SetCurrentFrame(m_CurrentAnimation.frames[m_CurrentAnimation.currentFrame]);
	}
}

void AnimationStateNode::Exit(Blackboard* pBlackboard)
{
	if (!pBlackboard || m_CurrentAnimation.spriteComponent == nullptr)
	{
		std::cerr << "AnimationStateNode::Exit() pBlackboard or spriteComponent is nullptr\n";
		return;
	}
	m_CurrentAnimation.currentFrame = 0;
	m_CurrentAnimation.timer = 0.f;
	m_CurrentAnimation.spriteComponent->SetCurrentFrame(m_CurrentAnimation.frames[m_CurrentAnimation.currentFrame]);
	m_IsFinished = false;
}

AnimatorComponent::AnimatorComponent(): m_pAnimationController{new AnimationController{new Blackboard{}}}
{

}

AnimatorComponent::~AnimatorComponent()
{
	delete m_pAnimationController;
	m_pAnimationController = nullptr;
}

AnimatorComponent::AnimatorComponent(const AnimatorComponent& other): AnimatorComponent{}
{
	m_pAnimationController = new AnimationController{*other.m_pAnimationController};
	m_pSpriteComponent = std::make_shared<SpriteComponent>(*other.m_pSpriteComponent);
}

AnimatorComponent::AnimatorComponent(AnimatorComponent&& other) noexcept
{
	m_pAnimationController = std::move(other.m_pAnimationController);
	other.m_pAnimationController = nullptr;
	m_pSpriteComponent = std::move(other.m_pSpriteComponent);
	other.m_pSpriteComponent = nullptr;
}

AnimatorComponent& AnimatorComponent::operator=(const AnimatorComponent& other)
{
	if(this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(other);
	delete m_pAnimationController;
	m_pAnimationController = new AnimationController{*other.m_pAnimationController};
	m_pSpriteComponent = std::make_shared<SpriteComponent>(*other.m_pSpriteComponent);

	return *this;
}

AnimatorComponent& AnimatorComponent::operator=(AnimatorComponent&& other) noexcept
{
	if(this == &other)
	{
		return *this;
	}
	BaseComponent::operator=(std::move(other));
	delete m_pAnimationController;
	m_pAnimationController = std::move(other.m_pAnimationController);
	other.m_pAnimationController = nullptr;
	m_pSpriteComponent = std::move(other.m_pSpriteComponent);
	other.m_pSpriteComponent = nullptr;

	return *this;
}

std::shared_ptr<BaseComponent> AnimatorComponent::Clone() const
{
	return std::make_shared<AnimatorComponent>(*this);
}

void AnimatorComponent::Init()
{
	if(GetGameObject()->HasComponent<SpriteComponent>())
	{
		m_pSpriteComponent = GetGameObject()->GetComponent<SpriteComponent>();
		return;
	}
	std::cerr << "SpriteComponent not initialized for GameObject: " << GetGameObject() << "\n";
}

void AnimatorComponent::Update()
{
	BaseComponent::Update();
	m_pAnimationController->Update();
}

bool AnimatorComponent::CurrentAnimationIsFinished() const
{
	if(const auto animState{ dynamic_cast<AnimationStateNode*>(m_pAnimationController->GetCurrentState()) })
	{
		return animState->IsFinished();
	}
	return false;
}

void AnimatorComponent::AddAnimation(const Animation& anim) const
{
	m_pAnimationController->AddStateNode(new AnimationStateNode{ anim });
}

bool AnimatorComponent::SetStartAnimation(const Animation& anim) const
{
	if (const auto startNode{ m_pAnimationController->GetStateNode(anim.name) })
	{
		m_pAnimationController->SetStartNode(dynamic_cast<AnimationStateNode*>(startNode));
		return true;
	}
	return false;
}

void AnimatorComponent::AddTransition(const Animation& startAnim, const Animation& endAnim, FSMTransition* pTransition) const
{
	AnimationStateNode* pStartAnimState;
	AnimationStateNode* pEndAnimState;
	if(const auto startNode{ m_pAnimationController->GetStateNode(startAnim.name) })
	{
		pStartAnimState = dynamic_cast<AnimationStateNode*>(startNode);
	}
	else
	{
		pStartAnimState = new AnimationStateNode{startAnim};
	}
	if(const auto endNode{ m_pAnimationController->GetStateNode(endAnim.name) })
	{
		pEndAnimState = dynamic_cast<AnimationStateNode*>(endNode);
	}
	else
	{
		pEndAnimState = new AnimationStateNode{endAnim};
	}
	m_pAnimationController->AddTransition(pStartAnimState, pEndAnimState, pTransition);
}
