#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <cmath>
#include <memory>

class Cell : public sf::Drawable
{
public:
	Cell(int x, int y, int wScaled, int hScaled);
	Cell();
	~Cell();

	float m_fScore; //!< Final score.
	float m_gScore; //!< Geographical score.
	float m_hScore; //!< Heuristcal Score.

	int m_xPos; //!< X position in the grid.
	int m_yPos; //!< Y position in the grid.

	Cell * m_previous = nullptr;

	bool m_isAnObstacle = false; //!< Is this Cell an obstacle?

	std::shared_ptr<std::vector<Cell>> m_neighbours;

	void calculateNeighbours(std::vector< std::vector<Cell> > &grid);

	void calculateHeuristic(Cell otherCell);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setFillColour(sf::Color colour);

	void updatePosition();

	bool operator==(const Cell& cell);

private:
	

	sf::RectangleShape m_square = sf::RectangleShape();

	float widthScaled;
	float heightScaled;
};