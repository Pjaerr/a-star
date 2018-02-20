#include "Program.h"
#include <iostream>

/*!
*	Creates a 2D Vector with the given cols and rows and populates it with Cell objects.
*	It then calculates every neighbour for each Cell within the grid and returns the grid.
*	Works out the width and height scaling within Globals.h.
*/
std::vector< std::vector<Cell> > Program::createGrid(int cols, int rows)
{
	std::cout << "Creating a grid with " << cols << " columns and " << rows << " rows." << std::endl;

	/*Work out the width and height scaled so that the 2D vector can be shown on screen.*/
	float widthScaled = m_width / cols;
	float heightScaled = m_height / rows;

	std::vector< std::vector<Cell> > grid; //! The grid to be returned.

	grid.resize(cols); //! Initialise the top level vector with 'cols' spaces.

	for (int i = 0; i < cols; i++)
	{
		grid[i].resize(rows); //! For every top level vector, create 'rows' bottom level vectors.

		for (int j = 0; j < rows; j++)
		{
			grid[i][j] = Cell(i, j, widthScaled, heightScaled); //! For every vector in the 2D vector, create a new Cell object with an (x, y) of its position in the 2D array.
		}
	}

	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			grid[i][j].calculateNeighbours(grid); //! Calculate the neighbours for every Cell in the grid.
			std::cout << "Calculating neighbours for Cell " << "[" << i << "]" << " [" << j << "]" << std::endl;
		}
	}

	return grid;
}


///Called once when the program starts.
void Program::Start()
{
	grid = createGrid(1000, 1000);

	startCell = grid[0][0];
	endCell = grid[grid.size() - 1][grid[0].size() - 1];
	startCell.m_isAnObstacle = false;
	endCell.m_isAnObstacle = false;
	openSet.push_back(startCell);
}

///Called every frame before drawing.
void Program::Update()
{
	//Whilst the openset is not empty.
	if (openSet.size() > 0)
	{
		int bestFScoreIndex = 0;

		for (int i = 0; i < openSet.size(); i++) //For every Cell in the open set.
		{
			/*If the current Cell has a better f score than the current best f score in the open set,
			set the index of the current Cell to be the best f score.*/
			if (openSet.at(i).m_fScore < openSet.at(bestFScoreIndex).m_fScore)
			{
				bestFScoreIndex = i;
			}
		}

		Cell currentCell = openSet[bestFScoreIndex]; //The current best Cell.

		if (currentCell == endCell)
		{
			std::cout << "Path found." << std::endl;
		}
		else //We haven't found our goal yet.
		{
			//Remove the current cell from the open set.
			for (int i = openSet.size() - 1; i >= 0; i--)
			{
				if (openSet.at(i) == currentCell)
				{
					openSet.erase(openSet.begin() + i);
				}
			}

			closedSet.push_back(currentCell); //Add the current cell to the closed set.

			
			/*Calculate the f score for every neighbour of the currentCell.*/
			for (int i = 0; i < currentCell.m_neighbours->size(); i++)
			{
				Cell &neighbour = currentCell.m_neighbours->at(i);
				bool neighbourIsInClosedSet = false;

				/*Loop through the closed set to see if the neighbour is apart of it.*/
				for (int j = 0; j < closedSet.size(); j++)
				{
					if (closedSet.at(j) == neighbour)
					{
						neighbourIsInClosedSet = true;
					}
				}

				//if the neighbour isn't in the closed set, and is not an obstacle.
				if (!neighbourIsInClosedSet && !neighbour.m_isAnObstacle)
				{
					bool neighbourIsInOpenSet = false;

					/*The geometric distance to get to this neighbour is the current cell's g plus 1 as
					this neighbouring cell is 1 away from our current cell.*/
					float temporaryG = currentCell.m_gScore + 1;

					/*Is the neighbour already in the open set?*/
					for (int j = 0; j < openSet.size(); j++)
					{
						if (openSet.at(j) == neighbour)
						{
							neighbourIsInOpenSet = true;
						}
					}

					bool shouldChooseNewPath = false;

					if (!neighbourIsInOpenSet) //If the neighbour isn't in the open set.
					{
						neighbour.m_gScore = temporaryG; //Give it our temporary g score.
						shouldChooseNewPath = true; //neighbour wasn't in the openSet, so choose a new path.
						openSet.push_back(neighbour); //Add it to the open set.
					}
					else //The neighbour is already in the open set.
					{
						/*If our temporary g score is better than the neighbour's g score.*/
						if (temporaryG < neighbour.m_gScore)
						{
							neighbour.m_gScore = temporaryG; //Set the neighbour's g score to our temp g score.
							shouldChooseNewPath = true; //The neighbour was in the open set but the new g score is better than its previous, so choose a new path.
						}
					}

					if (shouldChooseNewPath)
					{
						neighbour.calculateHeuristic(endCell); //Calculate the heursitic from the neighbour -> endcell.

						//Calculate the neighbours final f score.
						neighbour.m_fScore = neighbour.m_gScore + neighbour.m_hScore; 

						//The cell this neighbour came from is our current cell
						neighbour.m_previous = &currentCell;
					}

					pathTaken = std::vector<Cell>();
					Cell &temp = currentCell;
					
					pathTaken.push_back(currentCell);

					while (temp.m_previous != nullptr)
					{
						std::cout << temp.m_xPos << "AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH , " << temp.m_yPos << std::endl;
						pathTaken.push_back(*temp.m_previous);
						temp = *temp.m_previous;
					}
				}
			}
		}
	}


	/*Move the Cell to its (x,y) position.*/
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[0].size(); j++)
		{
			grid[i][j].updatePosition();
		}
	}
}

///Called every frame between window.clear and window.display.
void Program::Render()
{
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[0].size(); j++)
		{
			if (grid[i][j].m_isAnObstacle)
			{
				grid[i][j].setFillColour(sf::Color(0.0f, 0.0f, 0.0f));
			}
			else
			{
				grid[i][j].setFillColour(sf::Color(255.0f, 255.0f, 255.0f));
			}

			m_window->draw(grid[i][j]);
		}
	}


	for (int i = 0; i < openSet.size(); i++)
	{
		openSet[i].setFillColour(sf::Color(0.0f, 255.0f, 0.0f));
		m_window->draw(openSet[i]);
	}

	for (int i = 0; i < closedSet.size(); i++)
	{
		closedSet[i].setFillColour(sf::Color(255.0f, 0.0f, 0.0f));
		m_window->draw(closedSet[i]);
	}

	for (int i = 0; i < pathTaken.size(); i++)
	{
		pathTaken[i].setFillColour(sf::Color(0.0f, 0.0f, 255.0f));
		m_window->draw(pathTaken[i]);
	}
}






void Program::run_loop()
{
	sf::Event event; //! Used when handling SFML events.

	Start(); //! Call the Start function once.

	while (m_window->isOpen())
	{
		while (m_window->pollEvent(event))
		{
			if (event.type == event.Closed)
			{
				m_window->close();
			}
		}

		Update();

		m_window->clear();
		Render(); //! Call the Render function every frame.
		m_window->display();
	}
}

///Default Constructor
Program::Program()
{
	m_window = new sf::RenderWindow(sf::VideoMode(800, 600), "AStar C++ Implementation");
	m_width = 800;
	m_height = 600;
}

///Deconstructor
Program::~Program()
{
	delete m_window;
}