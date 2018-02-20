#include "Cell.h"
#include <iostream>

Cell::~Cell()
{
	
}

Cell::Cell()
{

}

Cell::Cell(int x, int y, int wScaled, int hScaled)
{
	m_xPos = x;
	m_yPos = y;

	widthScaled = wScaled;
	heightScaled = hScaled;

	//Generates a random float between 0.0 and 1.0
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	if (r < 0.1) //10% of the time
	{
		m_isAnObstacle = true;
	}

	m_square = sf::RectangleShape(sf::Vector2f(widthScaled - 1, heightScaled - 1));
	m_neighbours = std::shared_ptr<std::vector<Cell>>(new std::vector<Cell>);
}

void Cell::calculateNeighbours(std::vector< std::vector<Cell> > &grid)
{
	/*Add the cell to the right and to the left of this cell as long as this cell isn't
	at the very edge of the grid.*/

	if (m_xPos < grid.size() - 1)
	{
		m_neighbours->push_back(grid[m_xPos + 1][m_yPos]);
	}

	if (m_xPos > 0)
	{
		m_neighbours->push_back(grid[m_xPos - 1][m_yPos]);
	}


	/*Add the cell above and below this cell as long as this cell isn't
	at the very top or bottom of the grid.*/

	if (m_yPos < grid[0].size() - 1)
	{
		m_neighbours->push_back(grid[m_xPos][m_yPos + 1]);
	}

	if (m_yPos > 0)
	{
		m_neighbours->push_back(grid[m_xPos][m_yPos - 1]);
	}

	//Add the neighbour to the top left
	if (m_xPos > 0 && m_yPos > 0)
	{
		m_neighbours->push_back(grid[m_xPos - 1][m_yPos - 1]);
	}

	//Add the neighbour to the top right.
	if (m_xPos < grid.size() - 1 && m_yPos > 0)
	{
		m_neighbours->push_back(grid[m_xPos + 1][m_yPos - 1]);
	}

	//Add the neighbour to the bottom left
	if (m_xPos > 0 && m_yPos < grid[0].size() - 1)
	{
		m_neighbours->push_back(grid[m_xPos - 1][m_yPos + 1]);
	}

	//Add the neighbour to the bottom right.
	if (m_xPos < grid.size() - 1 && m_yPos < grid[0].size() - 1)
	{
		m_neighbours->push_back(grid[m_xPos + 1][m_yPos + 1]);
	}
}

void Cell::calculateHeuristic(Cell otherCell)
{
	/*RAW EUCLIDEAN DISTANCE BETWEEN THIS CELL AND THE OTHER CELL.*/
	float x = pow((m_xPos - otherCell.m_xPos), 2);
	float y = pow((m_yPos - otherCell.m_yPos), 2);
	float distance = sqrt(x + y);
	m_hScore = distance;
}

void Cell::updatePosition()
{
	m_square.setPosition(sf::Vector2f(m_xPos * widthScaled, m_yPos * heightScaled));
}

void Cell::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_square);
}

void Cell::setFillColour(sf::Color colour)
{
	m_square.setFillColor(colour);
}

bool Cell::operator==(const Cell& cell)
{
	return (this->m_xPos == cell.m_xPos) && (this->m_yPos == cell.m_yPos);
}