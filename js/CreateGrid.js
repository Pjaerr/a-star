/**Creating the grid as a 2D Array and populating it with 'cells'. It also 
 * stores the values needed when drawing the cells depending upon the size
 * of the grid and the size of the canvas.*/
function createGrid(cols, rows)
{
    let arr = new Array(cols); //Creates a new array with the number of columns.

    for (let i = 0; i < cols; i++) //For the number of columns.
    {
        arr[i] = new Array(rows); //Make each column be a new array with the number of rows.
    }

    for (let i = 0; i < cols; i++)
    {
        for (let j = 0; j < rows; j++)
        {
            arr[i][j] = new Cell(i, j); //Populate each spot in the grid with a new Cell object.
        }
    }

    for (let i = 0; i < cols; i++)
    {
        for (let j = 0; j < rows; j++)
        {
            arr[i][j].calculateNeighbours(arr);
        }
    }


    //Setup canvas -> grid scaling for drawing the cells.
    w = canvas.width / cols;
    h = canvas.height / rows;

    return arr;
}