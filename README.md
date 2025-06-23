# Terminal Pathfinding Visualizer (C++)

This project is a terminal-based pathfinding visualizer implemented in C++. It allows users to input a grid, designate start and end points, add obstacles, and visualize the traversal of three popular pathfinding algorithms.

## Features

- Real-time console animation of pathfinding steps
- Supports 8-directional movement
- Color-coded grid visualization using ANSI escape sequences
- Three implemented algorithms:
  - Breadth-First Search (BFS)
  - Dijkstra’s Algorithm
  - A* (A-Star) Search

## Grid Representation

- `S` — Start (green)
- `E` — End (red)
- `#` — Wall (white)
- `.` — Visited node (white)
- `a` — Current processing node (blue)
- `*` — Path from start to end (magenta)

## How to Build and Run

### Prerequisites

- A C++11 (or higher) compatible compiler
- A terminal that supports ANSI escape sequences (most Unix terminals and modern Windows terminals)

**Usage**
Enter the number of rows and columns for the grid.

Input the coordinates for the start and end positions.

Enter the number and positions of walls (obstacles).

Choose the algorithm to visualize:

1 — Breadth-First Search (BFS)

2 — Dijkstra's Algorithm

3 — A* Search

You can rerun different algorithms on the same grid layout without restarting the program.

**Example**

Enter number of rows and columns for the grid: 5 5
Enter START position (row col): 0 0
Enter END position (row col): 4 4
Enter number of WALLS: 3
Enter WALL 1 (row col): 1 1
Enter WALL 2 (row col): 1 2
Enter WALL 3 (row col): 1 3
Choose the algorithm for pathfinding:
1. Breadth-First Search (BFS)
2. Dijkstra's Algorithm
3. A* Search
Enter your choice (1-3), or any other number to exit: 2

**Known Limitations**
The program runs entirely in the terminal; resizing or interaction via GUI is not supported.

Input validation is basic — invalid coordinates may cause unexpected behavior.

### Compile

```bash
g++ -std=c++11 -o pathfinder pathfinder.cpp -pthread
