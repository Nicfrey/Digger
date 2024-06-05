#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

#include "Collider2D.h"
#include "InputManager.h"
#include "Minigin.h"
#include "Observer.h"
#include "QuadTree.h"
#include "SpatialGrid.h"
#include "Utils.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(std::string name) : m_name(std::move(name))
{
	m_QuadTree = std::make_shared<QuadTree>(Rectf{ {0.f,0.f},Minigin::m_Window.x,Minigin::m_Window.y });
	m_SpatialGrid = std::make_shared<SpatialGrid>(25, 25);
	//EventManager::GetInstance().AddEvent("ObjectsDestroyed", this, &Scene::UpdateQuadTree);
	//EventManager::GetInstance().AddEvent("ObjectMoving", this, &Scene::UpdateQuadTree);
}

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
		m_SpatialGrid->Add(object);
		//m_QuadTree-;>Insert(object);
		object->Init();
	}
}

void Scene::RenderGUI()
{
	m_SpatialGrid->RenderGUI();
	for (const auto& object : m_objects)
	{
		object->RenderGUI();
	}
}

void Scene::OnCollisionUpdate()
{

	for (auto& go : m_objects)
	{
		// m_QuadTree->Collide(go);
		m_SpatialGrid->OnCollisionUpdate(go);
	}
}

void Scene::UpdateQuadTree()
{
	m_QuadTree->Reset();
	for (const auto& object : m_objects)
	{
		m_QuadTree->Insert(object);
	}
}

std::shared_ptr<GameObject> Scene::GetGameObjectByTag(const std::string& tag) const
{
	for (const auto& object : m_objects)
	{
		if (object->GetTag() == tag)
		{
			return object;
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<GameObject>> Scene::GetGameObjectsByTag(const std::string& tag) const
{
	std::vector<std::shared_ptr<GameObject>> objectsWithTag;
	for (const auto& object : m_objects)
	{
		if (object->GetTag() == tag)
		{
			objectsWithTag.push_back(object);
		}
	}
	return objectsWithTag;
}

std::vector<std::shared_ptr<GameObject>> Scene::GetAllGameObject()
{
	return m_objects;
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Instantiate(std::shared_ptr<GameObject> object)
{
	m_ObjectsToBeAdded.emplace_back(std::move(object));
}

void Scene::Remove()
{
	m_SpatialGrid->Clear();
	std::vector<size_t> objectsToBeDestroyed;
	for(size_t i{}; i < m_objects.size(); ++i)
	{
		if(m_objects[i]->IsDestroyed())
		{
			objectsToBeDestroyed.push_back(i);
			m_objects[i]->OnDestroy();
		}
	}
	std::erase_if(m_objects, [this](const std::shared_ptr<GameObject>& object)
	{
		return object->IsDestroyed();
	});
}

void Scene::Remove(std::shared_ptr<GameObject>& object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
	object.reset();
}

void Scene::RemoveAll()
{
	for(const auto& object : m_objects)
	{
		object->Destroy();
		object->OnDestroy();
	}
	m_objects.clear();
	m_SpatialGrid->Clear();
	dae::InputManager::GetInstance().UnbindCommandObjects();
}

void Scene::Update()
{
	for (const auto& objectAdded : m_ObjectsToBeAdded)
	{
		objectAdded->Init();
		m_SpatialGrid->Add(objectAdded);
	}
	m_objects.insert(m_objects.end(), m_ObjectsToBeAdded.begin(), m_ObjectsToBeAdded.end());
	m_ObjectsToBeAdded.clear();
	for(const auto& object : m_objects)
	{
		object->Update();
		m_SpatialGrid->Update(object);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

