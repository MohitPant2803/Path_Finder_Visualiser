#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
#include <unordered_map>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
// usage: cout << RED << "*" << RESET << " ";

// struct Point {
//     int row, col;

//     bool operator==(const Point& other) const {
//         return row == other.row && col == other.col;
//     }

//     bool operator!=(const Point& other) const {
//         return !(*this == other);
//     }
// };



enum CellType { EMPTY, START, END, WALL, PATH, VISITED, CURRENT };


struct Point {
    int row, col;
    bool operator==(const Point& other) const {
        return row == other.row && col == other.col;
    }
};

// Needed for unordered_map key
struct Hash {
    size_t operator()(const Point& p) const {
        return hash<int>()(p.row * 1000 + p.col);
    }
};

bool isValid(int row, int col, int ROWS, int COLS) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

void displayGrid(const vector<vector<CellType>>& grid, int ROWS, int COLS) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            switch (grid[i][j]) {
                case EMPTY:
                    cout << WHITE << ". " << RESET;
                    break;
                case START:
                    cout << GREEN << "S " << RESET;
                    break;
                case END:
                    cout << RED << "E " << RESET;
                    break;
                case WALL:
                    cout << WHITE << "# " << RESET;
                    break;
                case PATH:
                    cout << MAGENTA << "* " << RESET;  // Changed from CYAN to MAGENTA
                    break;
                case VISITED:
                    cout << WHITE << ". " << RESET;
                    break;
                case CURRENT:
                    cout << BLUE << "a " << RESET;
                    break;
            }
        }
        cout << '\n';
    }
}





bool bfs(vector<vector<CellType>>& grid, Point start, Point end, int ROWS, int COLS) {
    queue<Point> q;
    unordered_map<Point, Point, Hash> parent;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));

    q.push(start);
    visited[start.row][start.col] = true;

    int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1}; 
    int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};

    cout << "\033[2J\033[H";  // Clear screen once

    while (!q.empty()) {
        Point current = q.front();
        q.pop();

        int r = current.row;
        int c = current.col;

        // Clear previous PATH and CURRENT
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < COLS; ++j)
                if (grid[i][j] == PATH || grid[i][j] == CURRENT)
                    grid[i][j] = EMPTY;

        // Update live path from current back to start
        Point temp = {r, c};
        while (parent.count(temp) && !(temp == start)) {
            if (grid[temp.row][temp.col] != START && grid[temp.row][temp.col] != END)
                grid[temp.row][temp.col] = PATH;
            temp = parent[temp];
        }

        // Mark current node as 'a'
        if (grid[r][c] != START && grid[r][c] != END)
            grid[r][c] = CURRENT;

        // Render
        cout << "\033[H";
        displayGrid(grid, ROWS, COLS);
        sleep_for(milliseconds(100));

        // Set current as visited (after display)
        if (grid[r][c] != START && grid[r][c] != END)
            grid[r][c] = VISITED;

        // If reached the end, stop
        if (r == end.row && c == end.col)
            return true;

        for (int d = 0; d < 8; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (isValid(nr, nc, ROWS, COLS) && !visited[nr][nc] && grid[nr][nc] != WALL) {
                visited[nr][nc] = true;
                parent[{nr, nc}] = current;
                q.push({nr, nc});
            }
        }
    }

    return false;
}


bool dijkstra(vector<vector<CellType>>& grid, Point start, Point end, int ROWS, int COLS) {
    vector<vector<int>> dist(ROWS, vector<int>(COLS, INT_MAX));
    unordered_map<Point, Point, Hash> parent;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;

    dist[start.row][start.col] = 0;
    pq.push({0, start.row, start.col});

    // 8-directional movement
    int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};

    // For clearing live path visual
    vector<Point> prevPath;

    cout << "\033[2J\033[H"; // Clear screen

    while (!pq.empty()) {
        int currDist, r, c;
        tie(currDist, r, c) = pq.top();
        pq.pop();

        if (visited[r][c]) continue;
        visited[r][c] = true;

        if (grid[r][c] != START && grid[r][c] != END)
            grid[r][c] = CURRENT;

        // Remove old live path
        for (const auto& p : prevPath)
            if (grid[p.row][p.col] == PATH)
                grid[p.row][p.col] = VISITED;
        prevPath.clear();

        // Show current live path
        Point trace = {r, c};
        while (!(trace== start) && parent.count(trace)) {
            Point prev = parent[trace];
            if (grid[prev.row][prev.col] != START && grid[prev.row][prev.col] != END) {
                grid[prev.row][prev.col] = PATH;
                prevPath.push_back(prev);
            }
            trace = prev;
        }

        // Display the grid
        cout << "\033[H";
        displayGrid(grid, ROWS, COLS);
        sleep_for(milliseconds(100));

        if (grid[r][c] != START && grid[r][c] != END)
            grid[r][c] = VISITED;

        if (r == end.row && c == end.col)
            return true;

        for (int d = 0; d < 8; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (isValid(nr, nc, ROWS, COLS) && grid[nr][nc] != WALL && !visited[nr][nc]) {
                int moveCost = (abs(dr[d]) + abs(dc[d]) == 2) ? 14 : 10; // Diagonal or straight
                int newDist = currDist + moveCost;

                if (newDist < dist[nr][nc]) {
                    dist[nr][nc] = newDist;
                    parent[{nr, nc}] = {r, c};
                    pq.push({newDist, nr, nc});
                }
            }
        }
    }

    return false;
}



int heuristic(Point a, Point b) {
    return abs(a.row - b.row) + abs(a.col - b.col); // Manhattan distance
}

bool aStar(vector<vector<CellType>>& grid, Point start, Point end, int ROWS, int COLS) {
    vector<vector<int>> gScore(ROWS, vector<int>(COLS, INT_MAX));
    unordered_map<Point, Point, Hash> parent;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
    gScore[start.row][start.col] = 0;

    auto cmp = [](const tuple<int, int, int>& a, const tuple<int, int, int>& b) {
        return get<0>(a) > get<0>(b);  // min-heap
    };
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, decltype(cmp)> pq(cmp);
    pq.push({heuristic(start, end), start.row, start.col});

    // 8-directional movement
    int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};

    vector<Point> prevPath;  // for clearing live path

    cout << "\033[2J\033[H";  // clear screen

    while (!pq.empty()) {
        auto [fScore, r, c] = pq.top();
        pq.pop();

        if (visited[r][c]) continue;
        visited[r][c] = true;

        if (grid[r][c] != START && grid[r][c] != END)
            grid[r][c] = CURRENT;

        // Clear previous live path
        for (auto& p : prevPath)
            if (grid[p.row][p.col] == PATH)
                grid[p.row][p.col] = VISITED;
        prevPath.clear();

        // Show current live path
        Point trace = {r, c};
        while (!(trace == start) && parent.count(trace)) {
            Point prev = parent[trace];
            if (grid[prev.row][prev.col] != START && grid[prev.row][prev.col] != END) {
                grid[prev.row][prev.col] = PATH;
                prevPath.push_back(prev);
            }
            trace = prev;
        }

        // Render
        cout << "\033[H";
        displayGrid(grid, ROWS, COLS);
        sleep_for(milliseconds(100));

        if (grid[r][c] != START && grid[r][c] != END)
            grid[r][c] = VISITED;

        if (r == end.row && c == end.col)
            return true;

        for (int d = 0; d < 8; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (isValid(nr, nc, ROWS, COLS) && !visited[nr][nc] && grid[nr][nc] != WALL) {
                int moveCost = (abs(dr[d]) + abs(dc[d]) == 2) ? 14 : 10;
                int tentativeG = gScore[r][c] + moveCost;

                if (tentativeG < gScore[nr][nc]) {
                    gScore[nr][nc] = tentativeG;
                    int f = tentativeG + heuristic({nr, nc}, end);
                    pq.push({f, nr, nc});
                    parent[{nr, nc}] = {r, c};
                }
            }
        }
    }

    return false;
}




int main() {
    int ROWS, COLS;
    cout << "Enter number of rows and columns for the grid: ";
    cin >> ROWS >> COLS;

    vector<vector<CellType>> grid(ROWS, vector<CellType>(COLS, EMPTY));
    int sr, sc, er, ec;

    cout << "Enter START position (row col): ";
    cin >> sr >> sc;
    grid[sr][sc] = START;

    cout << "Enter END position (row col): ";
    cin >> er >> ec;
    grid[er][ec] = END;

    cout << "Enter number of WALLS: ";
    int numWalls;
    cin >> numWalls;
    for (int i = 0; i < numWalls; ++i) {
        int wr, wc;
        cout << "Enter WALL " << i + 1 << " (row col): ";
        cin >> wr >> wc;
        if ((wr == sr && wc == sc) || (wr == er && wc == ec)) {
            cout << "Cannot place wall on START or END\n";
            --i;
            continue;
        }
        grid[wr][wc] = WALL;
    }

    cout << "\nInitial Grid:\n";
    displayGrid(grid, ROWS, COLS);

    Point start = {sr, sc};
    Point end = {er, ec};

    int choice;
while (true) {
   cout << WHITE << "\nChoose the algorithm for pathfinding:\n"
     << "1. Breadth-First Search (BFS)\n"
     << "2. Dijkstra's Algorithm\n"
     << "3. A* Search\n"
     << "Enter your choice (1-3), or any other number to exit: " << RESET;
    int choice;
    cin >> choice;

    if (choice != 1 && choice != 2 && choice != 3) {
        cout << "Exiting visualization. Thank you!\n";
        break;
    }

    // Reset grid (preserve walls, start, and end)
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            if (grid[i][j] == PATH)
                grid[i][j] = EMPTY;

    bool found = false;

    if (choice == 1) {
        found = bfs(grid, start, end, ROWS, COLS);
    } else if (choice == 2) {
        found = dijkstra(grid, start, end, ROWS, COLS);
    } else if (choice == 3) {
        found = aStar(grid, start, end, ROWS, COLS);
    }

    if (found) {
        cout << GREEN << "Path Found!\n" << RESET;
    } else {
        cout << RED << "No Path Found.\n" << RESET;
    }

    displayGrid(grid, ROWS, COLS);
}


    return 0;
}
