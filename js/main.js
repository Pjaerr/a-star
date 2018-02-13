document.addEventListener("click", (event) => 
{
    let mouseX = event.clientX;
    let mouseY = event.clientY;

    start(parseInt(mouseX / w), parseInt(mouseY / h));
});

let grid = createGrid(50, 50);

let openSet = [];
let closedSet = [];

let startCell = grid[0][0];
let endCell = new Cell(0, 0);

let pathTaken = [];

/**start is called once on program load.*/
function start(x, y)
{
    grid = createGrid(50, 50);
    openSet = [];
    closedSet = [];
    pathTaken = [];
    startCell = grid[0][0]; //Start node is the top left of the grid.
    endCell = grid[x][y];

    startCell.isAnObstacle = false;
    endCell.isAnObstacle = false;

    openSet.push(startCell);
}

/**update is called every frame.*/
function update()
{
    canvas.width = canvas.width; //Clear canvas.

    if (openSet.length > 0) //While openSet is not empty
    {
        let indexOfCurrentLowestValue = 0; //The index in the openSet that has the lowest f.

        for (let i = 0; i < openSet.length; i++)
        {
            /*Check each cell in the openSet to see if it has a lower f value than
            the current lowest f value.*/
            if (openSet[i].f < openSet[indexOfCurrentLowestValue].f)
            {
                indexOfCurrentLowestValue = i; //If it does, set the index of the cell to be the new lowest index.
            }
        }


        let currentCell = openSet[indexOfCurrentLowestValue]; //The current cell is the one in the openSet with the lowest value.

        if (currentCell === endCell) //We have reached the end cell.
        {

        }
        else //We haven't reached the end cell yet.
        {
            /*Take the current cell out of the open set, and add it to the closed set.*/
            for (let i = openSet.length - 1; i >= 0; i--)
            {
                if (openSet[i] === currentCell)
                {
                    openSet.splice(i, 1); //Remove the current cell from the openSet
                }
            }

            closedSet.push(currentCell); //Add the current cell to the closed set.


            /**Calculate the f score for every neighbour of the current cell.*/
            for (let i = 0; i < currentCell.neighbours.length; i++) //For every neighbour of the current cell.
            {
                let neighbour = currentCell.neighbours[i]; //The current neighbour.

                let neighbourIsInClosedSet = false;

                /*Loop through the closedSet and see if the neighbour is apart of it.*/
                for (let j = 0; j < closedSet.length; j++)
                {
                    if (closedSet[j] === neighbour)
                    {
                        neighbourIsInClosedSet = true;
                    }
                }

                //If the neighbour is not in the closed set.
                if (!neighbourIsInClosedSet && !neighbour.isAnObstacle)
                {
                    let neighbourIsInOpenSet = false;

                    /*The geometric distance to get to this neighbour is the current cell's g plus 1 as
                    this neighbouring cell is 1 away from our current cell.*/
                    let temporaryG = currentCell.g + 1;

                    for (let j = 0; j < openSet.length; j++)
                    {
                        if (openSet[j] === neighbour) //If the neighbour is in the open set.
                        {
                            neighbourIsInOpenSet = true;
                        }
                    }

                    let newPath = false;
                    if (!neighbourIsInOpenSet) //If the neighbour is not in the open set.
                    {
                        neighbour.g = temporaryG; //Give it our temp g score.
                        newPath = true; //neighbour wasn't in the openSet, so create a newPath.
                        openSet.push(neighbour); //Add it to the open set.
                    }
                    else //If the neighbour is in the open set.
                    {
                        if (temporaryG < neighbour.g) //Our temp g score is better than the neighbour's current g score.
                        {
                            neighbour.g = temporaryG; //Set the neighbour's g to be our g score.
                            newPath = true; //The neighbour was in the open set, but the distance to it is better, generate a new path.
                        }
                    }

                    if (newPath) //If we have chosen to create a new path.
                    {
                        //Calculate the heuristic score for the current neighbour.
                        neighbour.calculateHeuristicScore(endCell);

                        neighbour.f = neighbour.g + neighbour.h; //Calculate the neighbour's final f score.

                        neighbour.previous = currentCell; //The cell that this neighbour came from is our current cell.
                    }


                    pathTaken = [];
                    let temp = currentCell;

                    pathTaken.push(temp);

                    while (temp.previous)
                    {
                        pathTaken.push(temp.previous);
                        temp = temp.previous;
                    }
                }
            }
        }
    }

    //Draw the grid.
    drawGrid();
}



/**Draws the grid and changes their colour depending upon what set each cell is in.*/
function drawGrid()
{
    ctx.strokeStyle = "black";

    for (let i = 0; i < grid.length; i++)
    {
        for (let j = 0; j < grid[0].length; j++)
        {
            grid[i][j].isAnObstacle ? grid[i][j].draw("black") : grid[i][j].draw("white"); //Call each cell in the grid's draw function.
        }
    }

    //Draw cells that are in the open set.
    for (let i = 0; i < openSet.length; i++)
    {
        openSet[i].draw("green")
    }

    //Draw cells that are in the closed set.
    for (let i = 0; i < closedSet.length; i++)
    {
        closedSet[i].draw("red");
    }

    for (let i = 0; i < pathTaken.length; i++)
    {
        pathTaken[i].draw("blue");
    }

    if (endCell !== null)
    {
        endCell.draw("yellow");
    }
}