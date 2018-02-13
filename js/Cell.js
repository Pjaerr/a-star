/*The width and height that are used as 'scaling' when drawing cells to the canvas.
These will be set in the createGrid function depending upon both the size of the grid
being created, and then dimensions of the canvas.*/
let w = 0;
let h = 0;

function Cell(x, y)
{
    this.f = 0; //Overall cost.
    this.g = 0; //Geographical Distance Cost.
    this.h = 0; //Heuristical Cost.

    //Position
    this.x = x;
    this.y = y;

    this.previous;

    this.neighbours = [];

    this.isAnObstacle = false;


    if ((Math.random() * 1) < 0.1)
    {
        this.isAnObstacle = true;
    }

    //Draws this cell on the canvas via w and h scale set via createGrid()
    this.draw = function (colour)
    {
        ctx.fillStyle = colour;
        ctx.fillRect(this.x * w, this.y * h, w - 1, h - 1);
        ctx.strokeRect(this.x * w, this.y * h, w, h);
    }

    this.calculateNeighbours = function (grid)
    {
        /*Add the cell to the right and to the left of this cell as long as this cell isn't
        at the very edge of the grid.*/

        if (this.x < grid.length - 1)
        {
            this.neighbours.push(grid[this.x + 1][this.y]);
        }

        if (this.x > 0)
        {
            this.neighbours.push(grid[this.x - 1][this.y]);
        }


        /*Add the cell above and below this cell as long as this cell isn't 
        at the very top or bottom of the grid.*/

        if (this.y < grid[0].length - 1)
        {
            this.neighbours.push(grid[this.x][this.y + 1]);
        }

        if (this.y > 0)
        {
            this.neighbours.push(grid[this.x][this.y - 1]);
        }

        //Add the neighbour to the top left
        if (this.x > 0 && this.y > 0)
        {
            this.neighbours.push(grid[this.x - 1][this.y - 1]);
        }

        //Add the neighbour to the top right.
        if (this.x < grid.length - 1 && this.y > 0)
        {
            this.neighbours.push(grid[this.x + 1][this.y - 1]);
        }

        //Add the neighbour to the bottom left
        if (this.x > 0 && this.y < grid[0].length - 1)
        {
            this.neighbours.push(grid[this.x - 1][this.y + 1]);
        }

        //Add the neighbour to the bottom right.
        if (this.x < grid.length - 1 && this.y < grid[0].length - 1)
        {
            this.neighbours.push(grid[this.x + 1][this.y + 1]);
        }

    }

    /**Calculate the heuristic score of this cell with another cell.*/
    this.calculateHeuristicScore = function (otherCell)
    {
        /*RAW EUCLIDEAN DISTANCE BETWEEN THIS CELL AND THE OTHER CELL.*/
        let x = Math.pow((this.x - otherCell.x), 2);
        let y = Math.pow((this.y - otherCell.y), 2);
        let distance = Math.sqrt(x + y);
        this.h = distance;

        /*Manhattan distance (Calculates distance along the x and up/down the y.
        let distance = Math.abs(this.x - otherCell.x) + Math.abs(this.y - otherCell.y);
        this.h = distance;*/
    }
}