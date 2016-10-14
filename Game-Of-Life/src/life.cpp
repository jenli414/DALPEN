/* Lab 1: Game of Life
 * jenli414
 * sabse455 */

#include <iostream>
#include<fstream>
#include<string>
#include "grid.h"
#include "lifeutil.h"

using namespace std;

// Functiontion declarations.
void printWelcomeMessage();
void readFile(Grid<char>& grid);
void printGrid(const Grid<char>& grid);
void setNextGrid(Grid<char>& grid);
int getNeighbours(const Grid<char>& grid, const int& row, const int& column);
char getNewState(const char& currentState, const int& neighbours);
void animate(Grid<char>& grid);
void tick(Grid<char>& grid);
void askUser(char& nextUserAction);
void printExitMessage();


/* Main part of the program. Keeps track of what to do
 * next and calls appropriate functions. */
int main() {
    printWelcomeMessage();
    Grid<char> grid;
    readFile(grid);
    cout << endl;
    char nextUserAction;
    printGrid(grid);
    askUser(nextUserAction);
    bool run = true;
    while(run) {
        switch (nextUserAction) {
            case 'a':
                animate(grid);
                askUser(nextUserAction);
                break;
            case 't':
                tick(grid);
                askUser(nextUserAction);
                break;
            case 'q':
                run = false;
                break;
        }
    }
    printExitMessage();
    return 0;
}

// Prints welcome message.
void printWelcomeMessage() {
    cout << "Welcome to the TDDD86 Game of Life,\n"
    "a simulation of the lifecycle of a bacteria colony.\n"
    "Cells (X) live and die by the following rules:\n"
    " - A cell with 1 or fewer neighbours dies.\n"
    " - Locations with 2 neighbours remain stable.\n"
    " - Locationtos with 3 neighbours will create life.\n"
    " - A cell with 4 or more neightbours dies.\n" << endl;
}

/* Ask user for filename, reads that file, sets the grid to
 * the correct size and sets the elements according to it. */
void readFile(Grid<char>& grid) {
    //get file name from user
    string fileName;
    cout << "Grid input file name?: ";
    cin >> fileName;
    //load file
    ifstream input;
    input.open(fileName);
    //extract info from file
    int rows;
    int cols;
    int rownumber = 0;
    string fileText;
    while (getline(input, fileText)) {
        if (rownumber == 0) {
            stringstream convert(fileText);
            convert >> rows;
        } else if (rownumber == 1) {
            stringstream convert(fileText);
            convert >> cols;
            grid.resize(rows, cols);
        } else if (rownumber <= rows + 1) {
            for (int column = 0; column < cols; column++) {
                int gridRow = rownumber - 2;
                grid.set(gridRow, column, fileText[column]);
            }
        }
        ++rownumber;
    }
    input.close();
}

// Prints the grid.
void printGrid(const Grid<char>& grid) {
   string gridString;
   for (int row = 0; row < grid.numRows(); row++) {
       for (int column = 0; column < grid.numCols(); column++) {
           gridString.push_back(grid.get(row, column));
       }
       gridString.append("\n");
   }
   cout << gridString << endl;
}

/* Sets the next grid according to the neighbour situation of each
 * cell with the help of getNeighbours() and getNewState().*/
void setNextGrid(Grid<char>& grid) {
    Grid<char> newGrid = grid;
    int neighbours;
    char newState;
    for (int row = 0; row < grid.numRows(); row++) {
        for (int column = 0; column < grid.numCols(); column++) {
            neighbours = getNeighbours(grid, row, column);
            newState = getNewState(grid.get(row, column), neighbours);
            newGrid.set(row, column, newState);
        }
    }
    grid = newGrid;
}

// Gets the number of neighbours of a cell in the grid.
int getNeighbours(const Grid<char>& grid, const int& row, const int& column) {
    int neighbours = 0;
    int currentSquareRow;
    int currentSquareColumn;
    for (int row_i = -1; row_i <= 1; row_i++) {
        for (int column_i = -1; column_i <= 1; column_i++) {
            currentSquareRow = row + row_i;
            currentSquareColumn = column + column_i;
            if (!(row_i == 0 && column_i == 0)) {
                if (grid.inBounds(currentSquareRow, currentSquareColumn)) {
                    if (grid.get(currentSquareRow, currentSquareColumn) == 'X') {
                        ++neighbours;
                    }
                }
            }
        }
    }
    return neighbours;
}

/* Gets the next state of a cell according to current state and number of
 * neighbours. */
char getNewState(const char& currentState, const int& neighbours) {
    if (neighbours == 2) {
        return currentState;
    } else if(neighbours == 3) {
        return 'X';
    } else {
        return '-';
    }
}

// Sets and prints a new grid repeatedly.
void animate(Grid<char>& grid) {
    while (true) {
        clearConsole();
        tick(grid);
        pause(100);
    }
}

// Set and prints a new grid.
void tick(Grid<char>& grid) {
    setNextGrid(grid);
    printGrid(grid);
}

// Asks the user about what to do next and sets nextUserAction accordningly.
void askUser(char& nextUserAction) {
    cout << "a)nimate, t)ick, q)uit? ";
    cin >> nextUserAction;
}

// Prints exit message.
void printExitMessage() {
    cout << "Have a nice life!" << endl;
}




























