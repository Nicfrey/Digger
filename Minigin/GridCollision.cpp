#include "GridCollision.h"

#include "BoxCollider2D.h"
#include "GameObject.h"

EntityCell::EntityCell(GridCollision* grid)
{
	m_pGrid = grid;
	m_pNext = nullptr;
	m_pPrev = nullptr;
	m_pGrid->Add(this);
}

EntityCell::~EntityCell()
{
	m_pGrid = nullptr;
	m_pNext = nullptr;
	m_pPrev = nullptr;
	m_pGameObject = nullptr;
}

std::shared_ptr<dae::GameObject>& EntityCell::GetGameObject()
{
	return m_pGameObject;
}

void EntityCell::SetGameObject(const std::shared_ptr<dae::GameObject>& shared)
{
	m_pGameObject = shared;
}

GridCollision::GridCollision()
{
	for (int i{0}; i < m_NUM_CELLS; ++i)
	{
		for (int j{0}; j < m_NUM_CELLS; ++j)
		{
			m_Cells[i][j] = nullptr;
		}
	}
}

GridCollision::~GridCollision()
{
	for (int i{}; i < m_NUM_CELLS; ++i)
	{
		for (int j{}; j < m_NUM_CELLS; ++j)
		{
			EntityCell* pUnit{ m_Cells[i][j] };
			delete pUnit;
			pUnit = nullptr;
		}
	}
}

void GridCollision::Add(const std::shared_ptr<dae::GameObject>& gameObject)
{
	if (!gameObject->HasComponent<BoxCollider2D>())
	{
		return;
	}

	EntityCell* pUnit{ new EntityCell(this) };
	pUnit->SetGameObject(gameObject);
}

void GridCollision::Add(EntityCell* pUnit)
{
	int x{static_cast<int>(pUnit->GetGameObject()->GetWorldPosition().x / m_SIZE)};
	int y{static_cast<int>(pUnit->GetGameObject()->GetWorldPosition().y / m_SIZE)};

	pUnit->m_pPrev = nullptr;
	pUnit->m_pNext = m_Cells[x][y];
	m_Cells[x][y] = pUnit;

	if (pUnit->m_pNext != nullptr)
	{
		pUnit->m_pNext->m_pPrev = pUnit;
	}
}

void GridCollision::HandleCollision()
{
	for(int x{}; x < m_NUM_CELLS; ++x)
	{
		for(int y{}; y < m_NUM_CELLS; ++y)
		{
			HandleCell(m_Cells[x][y]);
		}
	}
}

EntityCell* GridCollision::GetCell(const std::shared_ptr<dae::GameObject>& gameObject)
{
	int x{ static_cast<int>(gameObject->GetWorldPosition().x / m_SIZE) };
	int y{ static_cast<int>(gameObject->GetWorldPosition().y / m_SIZE) };

	return m_Cells[x][y];
}

void GridCollision::HandleCell(EntityCell* pEntityCell)
{
	while (pEntityCell != nullptr)
	{
		EntityCell* pCheck{ pEntityCell->m_pNext };
		while (pCheck != nullptr)
		{
			if (pCheck->GetGameObject() && pEntityCell->GetGameObject() && pCheck->GetGameObject() == pEntityCell->GetGameObject())
			{
				if (pCheck->GetGameObject()->HasComponent<BoxCollider2D>() && pEntityCell->GetGameObject()->HasComponent<BoxCollider2D>())
				{
					auto colliderCheck{ pCheck->GetGameObject()->GetComponent<BoxCollider2D>() };
					auto colliderEntityCell{ pEntityCell->GetGameObject()->GetComponent<BoxCollider2D>() };
					colliderCheck->IsOverlapping(pEntityCell->GetGameObject());
					colliderEntityCell->IsOverlapping(pCheck->GetGameObject());
				}
			}
			pCheck = pCheck->m_pNext;
		}
		pEntityCell = pEntityCell->m_pNext;
	}
}
