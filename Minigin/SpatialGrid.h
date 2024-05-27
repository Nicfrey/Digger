#pragma once
#include <memory>
#include <unordered_map>
#include <glm/vec2.hpp>

// Grid Collision based on the book Game Programming Patterns by Robert Nystrom

namespace dae
{
	class GameObject;
}



class SpatialGrid
{
public:
	~SpatialGrid() = default;
	SpatialGrid(const SpatialGrid& other) = delete;
	SpatialGrid(SpatialGrid&& other) = delete;
	SpatialGrid& operator=(const SpatialGrid& other) = delete;
	SpatialGrid& operator=(SpatialGrid&& other) = delete;

	SpatialGrid(int nbCellsWidth, int nbCellsHeight);
	void Init(const std::vector<std::shared_ptr<dae::GameObject>>& objects);
	void Add(const std::shared_ptr<dae::GameObject>& object);
	void OnCollisionUpdate(std::shared_ptr<dae::GameObject>& go) const;
	glm::vec2 FindPosInCell(const std::shared_ptr<dae::GameObject>& go);
	void Update(const std::shared_ptr<dae::GameObject>& go);
	void RenderGUI() const;
	void Clear();
private:
	struct Grid
	{
		std::vector<std::shared_ptr<dae::GameObject>> objects;
		glm::vec2 position;
		bool operator==(const Grid& other) const { return position == other.position;}
	};
	int m_CellWidth;
	int m_CellHeight;
	int m_NbCellsWidth;
	int m_NbCellsHeight;
	std::vector<Grid> m_Cells;
	Grid GetCell(const glm::vec2& pos) const;
	std::vector<Grid> GetCellsCollided(const std::shared_ptr<dae::GameObject>& go) const;
};


