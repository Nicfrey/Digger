#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextureManager.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Init()
{
	m_TextureManager->GetInstance().Init();
}

void dae::GameObject::Update()
{
	m_TextureManager->UpdatePosition(m_transform.GetPosition());
}

void dae::GameObject::Render() const
{
	m_TextureManager->Render();
}

std::shared_ptr<TextureManager> dae::GameObject::GetComponent() const
{
	return m_TextureManager;
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_TextureManager->SetTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

dae::GameObject::GameObject()
{
	// TODO loop to all components and init them
	m_TextureManager = std::make_shared<TextureManager>();
	m_TextureManager->Init();
}
