#include <iostream>
#include <vector>
using namespace std;

enum CellType { EMPTY, START, END, WALL, PATH };

bool isValid(int row, int col, int ROWS, int COLS) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

void displayGrid(const vector<vector<CellType>>& grid, int ROWS, int COLS) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            switch (grid[i][j]) {
                case EMPTY: cout << ". "; break;
                case START: cout << "S "; break;
                case END:   cout << "E "; break;
                case WALL:  cout << "# "; break;
                case PATH:  cout << "* "; break;
            }
        }
        cout << endl;
    }
}

int main() {
    int ROWS, COLS;
    cout << "Enter number of rows and columns for the grid: ";
    cin >> ROWS >> COLS;

    vector<vector<CellType>> grid(ROWS, vector<CellType>(COLS, EMPTY));
    int sr, sc, er, ec;

    cout << "Enter START position (row col): ";
    cin >> sr >> sc;
    if (!isValid(sr, sc, ROWS, COLS)) {
        cout << " Invalid START position!" << endl;
        return 1;
    }
    grid[sr][sc] = START;

    cout << "Enter END position (row col): ";
    cin >> er >> ec;
    if (!isValid(er, ec, ROWS, COLS)) {
        cout << " Invalid END position!" << endl;
        return 1;
    }
    if (sr == er && sc == ec) {
        cout << " START and END cannot be the same!" << endl;
        return 1;
    }
    grid[er][ec] = END;

    cout << "\n Now, enter WALL positions (they block the path).\n";
    cout << "  Do NOT place walls on START or END positions.\n";

    int numWalls;
    cout << "Enter number of WALLS: ";
    cin >> numWalls;

    for (int i = 0; i < numWalls; ++i) {
        int wr, wc;
        cout << "Enter WALL " << i + 1 << " (row col): ";
        cin >> wr >> wc;

        if (!isValid(wr, wc, ROWS, COLS)) {
            cout << "   Invalid position, try again.\n";
            --i;
        } else if (grid[wr][wc] == START || grid[wr][wc] == END) {
            cout << "   Cannot place a wall on START or END.\n";
            --i;
        } else if (grid[wr][wc] == WALL) {
            cout << "    Wall already exists here. Try another spot.\n";
            --i;
        } else {
            grid[wr][wc] = WALL;
        }
    }

    cout << "\n Final Grid Setup:\n";
    displayGrid(grid, ROWS, COLS);

    return 0;
}
