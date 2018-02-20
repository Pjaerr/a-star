#pragma once
#include <SFML\Graphics.hpp>
#include "Cell.h"

class Program
{
public:
	Program();
	~Program();

	void run_loop();

private:
	sf::RenderWindow * m_window;

	int m_width;
	int m_height;

	void Start();
	void Update();
	void Render();

	std::vector< std::vector<Cell> > createGrid(int cols, int rows);
	std::vector< std::vector<Cell> > grid;

	std::vector<Cell> openSet;
	std::vector<Cell> closedSet;

	std::vector<Cell> pathTaken;

	Cell startCell = Cell();
	Cell endCell = Cell();
};