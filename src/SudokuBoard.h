#include <stdlib.h>
#include <vector>
#include <queue>
#include <string>
class SudokuBoard {
    public: 
        struct Cell {
            int row;
            int col;
            int val;
        };
        SudokuBoard(const std::string& filename, int n);
        int getGreedy(Cell cell);
        bool hasGreedy(Cell first, Cell second);
        bool solveGreedy();
        bool solveBacktracking(std::vector<std::vector<int> >& adjMat, std::vector<std::vector<int> >& board);
        Cell getNextCell();
        void populateVisitingOrder();
        bool graphColoredFully();
        void displayBoard();
        void populateAdjacencyMatrix();

        std::vector<Cell> getNeighbors(Cell cell) {
            std::vector<Cell> neighbors;
            int current = cell.row * 9 + cell.col;
            for(int i = 0; i < 81; i++) {
                if(adjacency_[current][i] == 1) {
                    // neighbor found
                    int neighbor_row = i / 9;
                    int neighbor_col = i % 9;
                    int color = cells_[neighbor_row][neighbor_col];
                    Cell cell;
                    cell.row = neighbor_row;
                    cell.col = neighbor_col;
                    cell.val = color;
                    neighbors.push_back(cell);
                }
            }
            return neighbors;
        }
        std::vector<std::vector<int> >& getAdjacency() {
            return adjacency_;
        }
        std::vector<std::vector<int> >& getCells() {
            return cells_;
        }
        void recursiveGraphColor(Cell cell);
        bool neighborsHaveColor(Cell cell, int color);
        bool canColor(Cell cell, int color);
        bool isValidCoordinate(int row, int col);

        std::vector<int> parseBoardToVector(int line, const std::string& filename);
        void displayAdjacency();
        void reset();
        // helper function for populating adjacency matrix
        void add_edge(int u, int v);
        std::string DFS();
        std::string toString();
        
    private: 
        std::vector<std::vector<int> > visited_ = std::vector<std::vector<int> >(9, std::vector<int>(9, 0));
        std::vector<std::vector<int> > beforeSolve_ = std::vector<std::vector<int> >(9, std::vector<int>(9, 0));
        std::vector<std::vector<int> > cells_ = std::vector<std::vector<int> >(9, std::vector<int>(9, 0));
        std::queue<Cell> visiting_order_;
        std::vector<std::vector<int> > adjacency_ = std::vector<std::vector<int> >(81, std::vector<int>(81, 0));
        std::string res = "";
};

std::vector<std::string> get_substrs(const std::string& str, char delimiter);
std::string removeNonNumber(std::string line);