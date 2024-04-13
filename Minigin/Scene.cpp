#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

#include "Collider2D.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::FixedUpdate()
{
	for (const auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Init()
{
	for (const auto& object : m_objects)
	{
		object->Init();
	}
}

void Scene::RenderGUI()
{
	for (const auto& object : m_objects)
	{
		object->RenderGUI();
	}
}

void Scene::OnCollisionUpdate()
{
	for(auto& go : m_objects)
	{
		if(go->HasComponent<ColliderComponent>())
		{
			for(auto& goOther : m_objects)
			{
				if(go == goOther)
					continue;

				if (std::shared_ptr otherCollider{ goOther->GetComponent<Collider2D>() })
				{
					otherCollider->IsOverlapping(go);
					auto goCollider{ go->GetComponent<Collider2D>() };
					goCollider->IsOverlapping(goOther);
				}
			}
		}
	}
}

std::vector<GameObject*> Scene::GetGameObjectsWithComponent() const
{
	std::vector<GameObject*> objectsWithComponent;
	for (const auto& object : m_objects)
	{
		if (object->HasComponent<ColliderComponent>())
		{
			objectsWithComponent.push_back(object.get());
		}
	}
	return objectsWithComponent;
}

GameObject* Scene::GetGameObjectByTag(const std::string& tag) const
{
	for (const auto& object : m_objects)
	{
		if (object->GetTag() == tag)
		{
			return object.get();
		}
	}
	return nullptr;
}

std::vector<GameObject*> Scene::GetGameObjectsByTag(const std::string& tag) const
{
	std::vector<GameObject*> objectsWithTag;
	for (const auto& object : m_objects)
	{
		if (object->GetTag() == tag)
		{
			objectsWithTag.push_back(object.get());
		}
	}
	return objectsWithTag;
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject>& object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
	object.reset();
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	for(const auto& object : m_objects)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

