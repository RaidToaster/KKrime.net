#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <map>
#include <utility>


class Cell {
public:
    int x, y;
    Cell(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Cell &other) const { return x == other.x && y == other.y; }
    bool operator<(const Cell &other) const { return x < other.x || (x == other.x && y < other.y); }
};

class Maze {
private:
    int width, height;
    std::vector<std::vector<int> > maze;
    std::vector<int> parent;
    std::vector<int> rank;

    int find(int cell) {
        if (parent[cell] != cell)
            parent[cell] = find(parent[cell]);
        return parent[cell];
    }

    void unionSets(int cell1, int cell2) {
        int root1 = find(cell1);
        int root2 = find(cell2);

        if (root1 != root2) {
            if (rank[root1] > rank[root2]) {
                parent[root2] = root1;
            } else if (rank[root1] < rank[root2]) {
                parent[root1] = root2;
            } else {
                parent[root2] = root1;
                rank[root1]++;
            }
        }
    }

    void initializeDisjointSets() {
        int numCells = (width / 2) * (height / 2);
        parent.resize(numCells);
        rank.resize(numCells, 0);

        for (int i = 0; i < numCells; ++i) {
            parent[i] = i;
        }
    }

    void generateMaze() {
        maze.assign(height, std::vector<int>(width, 1));

        std::vector<std::pair<Cell, Cell> > edges;
        for (int y = 1; y < height; y += 2) {
            for (int x = 1; x < width; x += 2) {
                if (x + 2 < width)
                    edges.push_back(std::make_pair(Cell(x, y), Cell(x + 2, y)));
                if (y + 2 < height)
                    edges.push_back(std::make_pair(Cell(x, y), Cell(x, y + 2)));
            }
        }
        srand(static_cast<unsigned int>(time(NULL)));
        std::random_shuffle(edges.begin(), edges.end());

        initializeDisjointSets();

        for (std::vector<std::pair<Cell, Cell> >::iterator it = edges.begin(); it != edges.end(); ++it) {
            int cell1 = (it->first.y / 2) * (width / 2) + (it->first.x / 2);
            int cell2 = (it->second.y / 2) * (width / 2) + (it->second.x / 2);
            if (find(cell1) != find(cell2)) {
                unionSets(cell1, cell2);
                maze[it->first.y][it->first.x] = 0;
                maze[it->second.y][it->second.x] = 0;
                maze[(it->first.y + it->second.y) / 2][(it->first.x + it->second.x) / 2] = 0;
            }
        }
        maze[1][0] = 0;
        maze[height - 2][width - 1] = 0;
    }

public:
    Maze(int width = 51, int height = 21) : width(width), height(height) {
        generateMaze();
    }
    
    std::vector<std::vector<int> > getMaze() const {
        return maze;
    }
    void displayMaze() const {
        const char horizontalTop = 220;
        const char verticalWall = 219;
        const char horizontalBottom = 223;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (maze[y][x] == 1) {
                    if (y == 0) {
                        printf("%c", horizontalTop);
                    } else if (y == height - 1) {
                        printf("%c", horizontalBottom);
                    } else {
                        printf("%c", verticalWall);
                    }
                } else {
                    printf(" ");
                }
            }
            printf("\n");
        }
    }


    bool solveMaze(std::vector<Cell> &path) {
        std::stack<Cell> s;
        std::vector<std::vector<bool> > visited(height, std::vector<bool>(width, false));
        std::map<Cell, Cell> parentMap;

        s.push(Cell(0, 1));
        visited[1][0] = true;

        while (!s.empty()) {
            Cell current = s.top();
            s.pop();

            if (current.x == width - 1 && current.y == height - 2) {
                Cell c = current;
                while (!(c == Cell(0, 1))) {
                    path.push_back(c);
                    c = parentMap[c];
                }
                path.push_back(Cell(0, 1));
                std::reverse(path.begin(), path.end());
                return true;
            }

            int dx[] = {0, 0, 1, -1};
            int dy[] = {1, -1, 0, 0};

            for (int i = 0; i < 4; i++) {
                int nx = current.x + dx[i];
                int ny = current.y + dy[i];

                if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                    maze[ny][nx] == 0 && !visited[ny][nx]) {
                    s.push(Cell(nx, ny));
                    visited[ny][nx] = true;
                    parentMap[Cell(nx, ny)] = current;
                }
            }
        }
        return false;
    }


    void displaySolution(const std::vector<Cell> &path) const {
        if (path.empty()) {
            std::cout << "No solution found.\n";
            return;
        }

        std::vector<std::vector<char> > charMaze(height, std::vector<char>(width));
        const char horizontalTop = 220;
        const char verticalWall = 219;
        const char horizontalBottom = 223;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (maze[y][x] == 1) {
                    if (y == 0) {
                        charMaze[y][x] = horizontalTop;
                    } else if (y == height - 1) {
                        charMaze[y][x] = horizontalBottom;
                    } else {
                        charMaze[y][x] = verticalWall;
                    }
                } else {
                    charMaze[y][x] = ' ';
                }
            }
        }


        for (size_t i = 0; i < path.size() - 1; ++i) {
            charMaze[path[i].y][path[i].x] = '*';
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                std::cout << charMaze[y][x];
            }
            std::cout << std::endl;
        }
    }
};
