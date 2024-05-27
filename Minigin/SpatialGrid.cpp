#include "SpatialGrid.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

#include "BoxCollider2D.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "imgui_plot.h"
#include "Minigin.h"
#include "../Digger/ProjectileComponent.h"

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


void SpatialGrid::OnCollisionUpdate(std::shared_ptr<dae::GameObject>& go) const
{
	if (!go->HasComponent<BoxCollider2D>())
	{
		return;
	}
	if (go->GetComponent<BoxCollider2D>()->GetIsStatic())
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
			boxCollider->IsOverlapping(otherObject);
			otherBoxCollider->IsOverlapping(go);
		}
	}
}

glm::vec2 SpatialGrid::FindPosInCell(const std::shared_ptr<dae::GameObject>& go)
{
	if (!go->HasComponent<BoxCollider2D>())
	{
		return glm::vec2{-1,-1};
	}
	if (go->GetComponent<BoxCollider2D>()->GetIsStatic())
	{
		return glm::vec2{ -1,-1 };
	}
	for(auto& grid : m_Cells)
	{
		if(std::ranges::find(grid.objects, go) != grid.objects.end())
		{
			return grid.position;
		}
	}
	return glm::vec2{ -1,-1 };
}

void SpatialGrid::Update(const std::shared_ptr<dae::GameObject>& go)
{
	if (!go->HasComponent<BoxCollider2D>())
	{
		return;
	}
	if (go->GetComponent<BoxCollider2D>()->GetIsStatic())
	{
		return;
	}
	const glm::vec2 pos = FindPosInCell(go);
	if(pos.x == -1 && pos.y == -1)
	{
		return;
	}
	if(pos != GetCell(go->GetWorldPosition()).position)
	{
		const auto cellX{static_cast<int>(pos.x) / m_CellWidth};
		const auto cellY{static_cast<int>(pos.y) / m_CellHeight};
		const auto newCellX{static_cast<int>(go->GetWorldPosition().x) / m_CellWidth};
		const auto newCellY{static_cast<int>(go->GetWorldPosition().y) / m_CellHeight};
		if(newCellX < 0 || newCellX >= m_NbCellsWidth || newCellY < 0 || newCellY >= m_NbCellsHeight)
		{
			return;
		}
		if(cellX < 0 || cellX >= m_NbCellsWidth || cellY < 0 || cellY >= m_NbCellsHeight)
		{
			return;
		}
		auto& cell{m_Cells[m_CellWidth * cellY + cellX]};
		auto& newCell{m_Cells[m_CellWidth * newCellY + newCellX]};
		std::erase_if(cell.objects, [&](const std::shared_ptr<dae::GameObject>& object)
		{
			return object == go;
		});
		newCell.objects.emplace_back(go);
	}
}



void SpatialGrid::RenderGUI() const
{
	ImGui::Begin("SpatialGrid");

	// Récupérer le contexte de dessin
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Définir la position de départ du dessin dans la fenêtre ImGui
	// ImVec2 canvasSize = ImGui::GetContentRegionAvail();

	// Couleur et épaisseur des lignes
	ImU32 gridColor = IM_COL32(200, 200, 200, 100);

	// Dessiner la grille
	for (int i = 0; i <= m_NbCellsWidth; ++i)
	{
		for (int j = 0; j <= m_NbCellsHeight; ++j)
		{
			ImVec2 cellTopLeft = ImVec2{static_cast<float>(j * m_CellWidth), static_cast<float>(i * m_CellHeight)};
			ImVec2 cellBottomRight = ImVec2(static_cast<float>(cellTopLeft.x + m_CellWidth),
			                                static_cast<float>(cellTopLeft.y + m_CellHeight));

			drawList->AddRect(cellTopLeft, cellBottomRight, gridColor, 0.0f, 0, 1.f);
		}
	}

	for (const auto& grid : m_Cells)
	{
		for (const auto& object : grid.objects)
		{
			const auto boxCollider{object->GetComponent<BoxCollider2D>()};
			const ImVec2 TopLeft{boxCollider->GetBoxCollider().topLeft.x, boxCollider->GetBoxCollider().topLeft.y};
			const ImVec2 bottomRight{
				boxCollider->GetBoxCollider().topLeft.x + boxCollider->GetBoxCollider().width,
				boxCollider->GetBoxCollider().topLeft.y + boxCollider->GetBoxCollider().height
			};
			drawList->AddRect(TopLeft, bottomRight, IM_COL32(0, 255, 0, 255));
		}
	}

	ImGui::End();
}

void SpatialGrid::Clear()
{
	for (auto& grid : m_Cells)
	{
		std::erase_if(grid.objects, [](const std::shared_ptr<dae::GameObject>& go)
		{
			return go->IsDestroyed();
		});
		if(!grid.objects.empty())
			grid.objects.erase(std::unique(grid.objects.begin(),grid.objects.end()),grid.objects.end());
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
	const glm::vec2 topLeft{boxCollider->GetBoxCollider().topLeft};
	const glm::vec2 topRight{topLeft + glm::vec2{boxCollider->GetBoxCollider().width, 0}};
	const glm::vec2 bottomLeft{topLeft + glm::vec2{0, boxCollider->GetBoxCollider().height}};
	const glm::vec2 bottomRight{
		topLeft + glm::vec2{boxCollider->GetBoxCollider().width, boxCollider->GetBoxCollider().height}
	};
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
