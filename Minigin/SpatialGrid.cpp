#include "SpatialGrid.h"

#include <algorithm>

#include "BoxCollider2D.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "Minigin.h"

SpatialGrid::SpatialGrid(int nbCellsWidth, int nbCellsHeight) : m_NbCellsWidth{nbCellsWidth},
                                                                m_NbCellsHeight{nbCellsHeight}
{
	m_CellWidth = static_cast<int>(dae::Minigin::m_Window.x) / m_NbCellsWidth;
	m_CellHeight = static_cast<int>(dae::Minigin::m_Window.y) / m_NbCellsHeight;
	for (int i{}; i <= m_NbCellsWidth; ++i)
	{
		for (int j{}; j <= m_NbCellsHeight; ++j)
		{
			m_Cells.emplace_back(Grid{
				std::vector<std::shared_ptr<dae::GameObject>>{}, glm::vec2{j * m_CellWidth, i * m_CellHeight}
			});
		}
	}
}

void SpatialGrid::Init(const std::vector<std::shared_ptr<dae::GameObject>>& objects)
{
	for (const auto& object : objects)
	{
		if (!object->HasComponent<ColliderComponent>())
		{
			continue;
		}
		const auto pos{object->GetWorldPosition()};
		const auto cellX{static_cast<int>(pos.x) / m_CellWidth};
		const auto cellY{static_cast<int>(pos.y) / m_CellHeight};
		m_Cells[m_CellWidth * cellY + cellX].objects.emplace_back(object);
	}
}

void SpatialGrid::Add(const std::shared_ptr<dae::GameObject>& object)
{
	if (!object->HasComponent<ColliderComponent>())
	{
		return;
	}
	const auto pos{object->GetWorldPosition()};
	const auto cellX{static_cast<int>(pos.x) / m_CellWidth};
	const auto cellY{static_cast<int>(pos.y) / m_CellHeight};
	if (cellX < 0 || cellX >= m_NbCellsWidth || cellY < 0 || cellY >= m_NbCellsHeight)
	{
		return;
	}
	m_Cells[m_CellWidth * cellY + cellX].objects.emplace_back(object);
}

void SpatialGrid::Update()
{
	for (auto& cell : m_Cells)
	{
		for (auto& object : cell.objects)
		{
			if (object->GetIsMoving())
			{
				const auto pos{object->GetWorldPosition()};
				const auto cellX{static_cast<int>(pos.x) / m_CellWidth};
				const auto cellY{static_cast<int>(pos.y) / m_CellHeight};
				if (cellX != static_cast<int>(cell.position.x) || static_cast<int>(cellY != cell.position.y))
				{
					auto it{std::find(cell.objects.begin(), cell.objects.end(), object)};
					if (it != cell.objects.end())
					{
						cell.objects.erase(it);
						m_Cells[m_CellWidth * cellY + cellX ].objects.emplace_back(object);
					}
				}
				object->SetIsNotMoving();
			}
		}
	}
}

void SpatialGrid::OnCollisionUpdate(std::shared_ptr<dae::GameObject>& go) const
{
	if(!go->HasComponent<ColliderComponent>())
	{
		return;
	}
	auto cells{GetCellsCollided(go)};
	for (auto& otherCell : cells)
	{
		for (auto& otherObject : otherCell.objects)
		{
			if (go == otherObject)
			{
				continue;
			}
			const auto boxCollider{go->GetComponent<BoxCollider2D>()};
			const auto otherBoxCollider{otherObject->GetComponent<BoxCollider2D>()};
			if(boxCollider->IsOverlapping(otherObject))
			{
				go->OnCollisionEnter(otherObject);
				otherObject->OnCollisionEnter(go);
			}
		}
	}
}

SpatialGrid::Grid SpatialGrid::GetCell(const glm::vec2& pos) const
{
	const auto cellX{static_cast<int>(pos.x) / m_CellWidth};
	const auto cellY{static_cast<int>(pos.y) / m_CellHeight};
	return m_Cells[m_CellWidth * cellY + cellX];
}

std::vector<SpatialGrid::Grid> SpatialGrid::GetCellsCollided(const std::shared_ptr<dae::GameObject>& go) const
{
	std::vector<Grid> cells;
	const auto boxCollider{go->GetComponent<BoxCollider2D>()};
	const auto bottomLeft{boxCollider->GetBoxCollider().bottomLeft};
	const auto topRight{
		bottomLeft + glm::vec2{boxCollider->GetBoxCollider().width, boxCollider->GetBoxCollider().height}
	};
	const auto bottomRight{bottomLeft + glm::vec2{boxCollider->GetBoxCollider().width, 0}};
	const auto topLeft{bottomLeft + glm::vec2{0, boxCollider->GetBoxCollider().height}};
	auto cellBottomLeft{GetCell(bottomLeft)};
	auto cellTopRight{GetCell(topRight)};
	auto cellBottomRight{GetCell(bottomRight)};
	auto cellTopLeft{GetCell(topLeft)};
	cells.emplace_back(cellBottomLeft);
	cells.emplace_back(cellTopRight);
	cells.emplace_back(cellBottomRight);
	cells.emplace_back(cellTopLeft);
	// Check if the cells are the same
	std::ranges::sort(cells, [](const Grid& rhs, const Grid& lhs)
	{
		return rhs.position.x < lhs.position.x && rhs.position.y < lhs.position.y;
	});
	cells.erase(std::unique(cells.begin(), cells.end()), cells.end());
	return cells;
}
