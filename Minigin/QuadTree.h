// Based on https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det/48330314#48330314
#pragma once
#include <memory>
#include <vector>

#include "Utils.h"

namespace dae
{
	class GameObject;
}

class QuadTree final
{
public:
	QuadTree(const Rectf& shape);
	~QuadTree();

	void Insert(const std::shared_ptr<dae::GameObject>& object);
	bool Contains(const std::shared_ptr<dae::GameObject>& object) const;
	void Reset();
	std::vector<QuadTree*> GetLeafs(const std::shared_ptr<dae::GameObject>& object);
	void Collide(std::shared_ptr<dae::GameObject>& object);
private:
	QuadTree* m_NordWestTree;
	QuadTree* m_NordEastTree;
	QuadTree* m_SouthWestTree;
	QuadTree* m_SouthEastTree;

	int m_Limit{ 4 };
	int m_DepthLimit{ 5 };
	bool m_IsLeaf{ false };
	std::vector<std::shared_ptr<dae::GameObject>> m_Objects;
	Rectf m_Shape;
	int m_Depth{};

	void ContainsAndInsertObject(const std::shared_ptr<dae::GameObject>& object) const;
};

