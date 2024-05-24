#pragma once
#include <memory>
#include <glm/vec2.hpp>

// Grid Collision based on the book Game Programming Patterns by Robert Nystrom

namespace dae
{
	class GameObject;
}

class EntityCell final
{
	friend class GridCollision;
public:
	EntityCell(GridCollision* grid);
	~EntityCell();
	std::shared_ptr<dae::GameObject>& GetGameObject();
	void SetGameObject(const std::shared_ptr<dae::GameObject>& shared);

private:
	std::shared_ptr<dae::GameObject> m_pGameObject;
	GridCollision* m_pGrid;
	EntityCell* m_pNext;
	EntityCell* m_pPrev;
};

class GridCollision final
{
public:
	GridCollision();
	~GridCollision();
	void Add(const std::shared_ptr<dae::GameObject>& gameObject);
	void Add(EntityCell* pUnit);
	void HandleCollision();
	EntityCell* GetCell(const std::shared_ptr<dae::GameObject>& gameObject);
private:
	static constexpr int m_SIZE{ 10 };
	static constexpr int m_NUM_CELLS{ 10 };
	EntityCell* m_Cells[m_NUM_CELLS][m_NUM_CELLS];
	void HandleCell(EntityCell* pEntityCell);
};

