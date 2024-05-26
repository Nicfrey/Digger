#include "QuadTree.h"

#include <set>

#include "BoxCollider2D.h"
#include "GameObject.h"
#include "imgui.h"

QuadTree::QuadTree(const Rectf& shape): m_Shape{shape}
{
	m_NordEastTree = nullptr;
	m_SouthEastTree = nullptr;
	m_NordWestTree = nullptr;
	m_SouthWestTree = nullptr;
	m_IsLeaf = true;
}

QuadTree::~QuadTree()
{
	if (!m_IsLeaf)
	{
		delete m_NordEastTree;
		delete m_NordWestTree;
		delete m_SouthEastTree;
		delete m_SouthWestTree;
	}
}

void QuadTree::Insert(const std::shared_ptr<dae::GameObject>& object)
{
	if (!object->HasComponent<ColliderComponent>())
	{
		return;
	}

	if(!m_IsLeaf)
	{
		ContainsAndInsertObject(object);
	}
	else
	{
		m_Objects.emplace_back(object);
		if(m_Objects.size() >= static_cast<size_t>(m_Limit))
		{
			if(m_Depth >= m_DepthLimit)
			{
				return;
			}
			m_IsLeaf = false;

			Rectf childShape{};
			childShape = Rectf{ m_Shape.topLeft, m_Shape.width / 2, m_Shape.height / 2 };
			m_NordWestTree = new QuadTree(childShape);
			m_NordWestTree->m_Depth = m_Depth + 1;
			childShape = Rectf{ glm::vec2{m_Shape.topLeft.x + m_Shape.width / 2, m_Shape.topLeft.y}, m_Shape.width / 2, m_Shape.height / 2 };
			m_NordEastTree = new QuadTree(childShape);
			m_NordEastTree->m_Depth = m_Depth + 1;
			childShape = Rectf{ glm::vec2{m_Shape.topLeft.x, m_Shape.topLeft.y + m_Shape.height / 2}, m_Shape.width / 2, m_Shape.height / 2 };
			m_SouthWestTree = new QuadTree(childShape);
			m_SouthWestTree->m_Depth = m_Depth + 1;
			childShape = Rectf{ glm::vec2{m_Shape.topLeft.x + m_Shape.width / 2, m_Shape.topLeft.y + m_Shape.height / 2}, m_Shape.width / 2, m_Shape.height / 2 };
			m_SouthEastTree = new QuadTree(childShape);
			m_SouthEastTree->m_Depth = m_Depth + 1;

			for (auto& obj : m_Objects)
			{
				ContainsAndInsertObject(obj);
			}
			m_Objects.clear();
		}
	}
}

bool QuadTree::Contains(const std::shared_ptr<dae::GameObject>& object) const
{
	if(object->HasComponent<Collider2D>())
	{
		const auto collider{ object->GetComponent<BoxCollider2D>() };
		const auto box{ collider->GetBoxCollider() };
		return IsOverlapping(m_Shape, box);
	}
	return false;
}

void QuadTree::Reset()
{
	m_Objects.clear();
	if (!m_IsLeaf)
	{
		delete m_NordEastTree;
		m_NordEastTree = nullptr;
		delete m_NordWestTree;
		m_NordWestTree = nullptr;
		delete m_SouthEastTree;
		m_SouthEastTree = nullptr;
		delete m_SouthWestTree;
		m_SouthWestTree = nullptr;
	}
	m_IsLeaf = true;
}

std::vector<QuadTree*> QuadTree::GetLeafs(const std::shared_ptr<dae::GameObject>& object)
{
	std::vector<QuadTree*> leafs;
	if (m_IsLeaf)
	{
		leafs.emplace_back(this);
	}
	else
	{
		if (m_NordWestTree->Contains(object))
		{
			std::vector<QuadTree*> temp{ m_NordWestTree->GetLeafs(object) };
			leafs.insert(leafs.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
		}
		if (m_NordEastTree->Contains(object))
		{
			std::vector<QuadTree*> temp{ m_NordEastTree->GetLeafs(object) };
			leafs.insert(leafs.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
		}
		if (m_SouthWestTree->Contains(object))
		{
			std::vector<QuadTree*> temp{ m_SouthWestTree->GetLeafs(object) };
			leafs.insert(leafs.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
		}
		if (m_SouthEastTree->Contains(object))
		{
			std::vector<QuadTree*> temp{ m_SouthEastTree->GetLeafs(object) };
			leafs.insert(leafs.end(), std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
		}
	}
	return leafs;
}

void QuadTree::Collide(std::shared_ptr<dae::GameObject>& object)
{
	auto leafs{ GetLeafs(object) };

	std::set<std::shared_ptr<dae::GameObject>> alreadyCollided;

	for (const auto& leaf: leafs)
	{
		for (auto& other: leaf->m_Objects)
		{
			if (other == object || alreadyCollided.contains(other))
			{
				continue;
			}
			if (object->HasComponent<Collider2D>() && other->HasComponent<Collider2D>())
			{
				const auto collider{ object->GetComponent<BoxCollider2D>() };
				const auto otherCollider{ other->GetComponent<BoxCollider2D>() };
				if (collider->IsOverlapping(other))
				{
					object->OnCollisionEnter(other);
					other->OnCollisionEnter(object);
					alreadyCollided.insert(other);
				}
			}
		}
	}
}

void QuadTree::ContainsAndInsertObject(const std::shared_ptr<dae::GameObject>& object) const
{
	if (m_NordEastTree->Contains(object))
	{
		m_NordEastTree->Insert(object);
		return;
	}
	if (m_NordWestTree->Contains(object))
	{
		m_NordWestTree->Insert(object);
		return;
	}
	if (m_SouthEastTree->Contains(object))
	{
		m_SouthEastTree->Insert(object);
		return;
	}
	if (m_SouthWestTree->Contains(object))
	{
		m_SouthWestTree->Insert(object);
	}
}
