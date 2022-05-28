#include "SudokuBoard.h"
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept> 
#include <set>
#include <stdio.h>
#include <ctype.h>
#include <algorithm>
#include <chrono>
#include <random>


void SudokuBoard::displayBoard() {
    std::cout << "--------- BOARD ---------" << std::endl;
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            std::cout << cells_[i][j] << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << "--------- BOARD ---------" << std::endl;
}
SudokuBoard::SudokuBoard(const std::string& filename, int n) {
    // parse csv into sudoku board graph representation

    std::vector<int> cells = parseBoardToVector(n, filename);
    // populate cells_ using 1D vector cells
    // cells should be size 81
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            cells_[i][j] = cells.at(i * 9 + j);
            beforeSolve_[i][j] = cells.at(i * 9 + j);
        }
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            visited_[i][j] = false;
        }
    }

    populateAdjacencyMatrix();
    populateVisitingOrder();
}

// returns how many adjacent cells are colored - higher number means less choices for the coloring of the given cell
int SudokuBoard::getGreedy(Cell cell) {
    int current = cell.row * 9 + cell.col;
    std::vector<int> colors;
    std::set<int> set_colors;
    for(int i = 0; i < 81; i++) {
        if(adjacency_[current][i] == 1) {
            // neighbor found
            int neighbor_row = i / 9;
            int neighor_col = i % 9;
            int neighbor = cells_[neighbor_row][neighor_col];
            if(neighbor != cell.val && neighbor != 0) {
                /// do something
                set_colors.insert(neighbor);
            }
        }
    }
    return set_colors.size();
}
bool SudokuBoard::hasGreedy(Cell first, Cell second) {
    int first_deg = getGreedy(first);
    int second_deg = getGreedy(second);
    if(first_deg > second_deg) {
        return true;
    }
    return false;
}
void SudokuBoard::populateVisitingOrder() {
    std::vector<Cell> order_vect;
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            Cell cell;
            cell.row = i;
            cell.col = j;
            cell.val = cells_[i][j];
            //if(cell.val == 0) { // only cells that aren't colored
                order_vect.push_back(cell);
            //}
        }
    }

    int i;
    Cell key;
    int j;
    for (i = 1; i < 81; i++){
        key = order_vect[i];
        j = i - 1;
 
        // Move elements of arr[0 till i-1], that are greater than key, to one position ahead of their current position
        while (j >= 0 && hasGreedy(order_vect[j], key)) {
            order_vect[j + 1] = order_vect[j];
            j = j - 1;
        }
        order_vect[j + 1] = key;
    }
    // order_vect is now sorted by saturation degree

    // shuffle portions of order_vect that have identical saturation degrees
    for(int degree = 8; degree >= 0; degree--) {
        int start_index = order_vect.size();
        int end_index = 0;
        bool degree_exists = false;
        for(int i = 0; i < (int)order_vect.size(); i++) {
            if(getGreedy(order_vect[i]) == degree) {
                degree_exists = true;
                if(i < start_index) {
                    start_index = i;
                }
                if(i > end_index) {
                    end_index = i;
                }
            }
        }
        if(degree_exists) {
            // now we have a start and end index for this degree: proceed to randomly shuffle this range
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::mt19937 eng(seed);
            std::shuffle(order_vect.begin() + start_index, order_vect.begin() + end_index, eng);//std::default_random_engine(seed)); 
        }
    }

    // convert order_vect into a queue
    for(int i = (int)(order_vect.size() - 1); i >= 0; i--) {
        if(order_vect[i].val == 0) {
            visiting_order_.push(order_vect[i]);
        }
    }
}

std::vector<int> SudokuBoard::parseBoardToVector(int board_num, const std::string& filename) {
    std::vector<std::string> datastr;
    std::ifstream csvFile(filename);

    if(!csvFile.is_open()){
        throw std::runtime_error("Could not open file");
    }

    std::string line;
    // will move past anything less than 163 characters;
    // 81 cells for quiz + 81 cells for solution + 1 comma = 163 characters;
    while (std::getline(csvFile, line)) {
        if (line.length() < 163) {
            for(int i = 0; i < board_num; i++) {
                std::getline(csvFile, line); // moves to line (board_num - # of lines skipped);
            }
            break;
        }
    }
    // moves quiz and solution to a vector;
    // datastr[0] = quiz, datastr[1] = solution
    // push "cleaned" quiz + solution to new vector;
    datastr = get_substrs(line, ',');
    std::vector<std::string> cleanedSudokuBoards;
    for (unsigned int i = 0; i < datastr.size(); i++) {
        std::string edit = removeNonNumber(datastr[i]);
        cleanedSudokuBoards.push_back(edit);
    }

    // if the quiz is not equal to 81 numbers, then it is not a valid Sudoku puzzle;
    // throw error;
    if (cleanedSudokuBoards[0].length() != 81) {
        throw std::logic_error("Invalid Board.");
    }

    std::vector<int> cells;
    for(int i = 0; i < 81; i++) {
        int val = cleanedSudokuBoards[0].at(i) - '0';
        cells.push_back(val);
    }
    // should now have 81-sized cells vector

    return cells;
}

void SudokuBoard::populateAdjacencyMatrix() {
    for(int i = 0; i < 81; i++) {
        for(int j = 0; j < 81; j++) {
            adjacency_[i][j] = 0;
        }
    }

    for(int r1 = 0; r1 < 9; r1++) {
        for(int c1 = 0; c1 < 9; c1++) {
            for(int r2 = 0; r2 < 9; r2++) {
        		for(int c2 = 0; c2 < 9; c2++) {	
					if(r1 == r2) {//same row check
						add_edge(9*r1 + c1, 9*r2 + c2);
					} else if(c1 == c2) {//same col check
						add_edge(9*r1 + c1, 9*r2 + c2);
					}

					//same 3x3 check
					int i1 = r1 / 3 * 3;
					int j1 = c1 / 3 * 3;
					int i2 = r2 / 3 * 3;
					int j2 = c2 / 3 * 3;
					if(i1 == i2 && j1 == j2) {
						add_edge(9*r1 + c1, 9*r2 + c2);
					}
					
				}
			}

        }
    }
}

void SudokuBoard::add_edge(int u, int v) { //function to add edge into the adjacency matrix
   adjacency_[u][v] = 1;
   adjacency_[v][u] = 1;
}


void SudokuBoard::displayAdjacency() {
   int count = 1;
   //the adjMat is a 81x81 matrix with each side representing all 81 cells
   //i.e the top axis would be cells[0],cells[1],cells[2],...
   //and the left axis would also be cells[0],cells[1],cells[2],...
   std::cout << "------ADJ MATRIX------" << std::endl;
   for(int i = 0; i < 81; i++) {
      for(int j = 0; j < 81; j++) {
         std::cout << adjacency_[i][j] << " ";
      }
	  count++;
      std::cout<< std::endl;
   }
   std::cout << "------ADJ MATRIX------" << std::endl;
}

bool SudokuBoard::isValidCoordinate(int row, int col) {
    if(row >= 0 && row <= 8 && col >= 0 && col <= 8) {
        return true;
    }
    return false;
}
bool SudokuBoard::neighborsHaveColor(Cell cell, int color) {
    // use adjacency matrix to determine if graph neighbors have color
    int current = cell.row * 9 + cell.col;
    for(int i = 0; i < 81; i++) {
        if(adjacency_[current][i] == 1) {
            // neighbor found
            int neighbor_row = i / 9;
            int neighor_col = i % 9;
            int neighbor = cells_[neighbor_row][neighor_col];
            if(neighbor == color && !(neighbor_row == cell.row && neighor_col == cell.col)) {
                //displayBoard();
                return true;
            }
        }
    }
    return false;
}
bool SudokuBoard::canColor(Cell cell, int color) {
    if(cells_[cell.row][cell.col] != 0 && cells_[cell.row][cell.col] != color) {
        return false;
    }
    if(neighborsHaveColor(cell, color)) {
        return false;
    }
    if(cells_[cell.row][cell.col] == color) {
        return true;
    }
    return true;
}
SudokuBoard::Cell SudokuBoard::getNextCell() {
    if(visiting_order_.empty()) {
        Cell cell;
        cell.row = -1;
        cell.col = -1;
        return cell;
    }
    Cell cell = visiting_order_.front();
    visiting_order_.pop();
    return cell;
}

void SudokuBoard::recursiveGraphColor(Cell cell) {
    for(int color = 1; color <= 9; color++) {
        if(canColor(cell, color)) {
            cells_[cell.row][cell.col] = color;
            break;
        }
    }
    Cell next_cell = getNextCell();
    if(!(next_cell.row == -1 && next_cell.col == -1)) {
        // recurse to the next vertex
        recursiveGraphColor(next_cell);
    }
    return;
}

bool SudokuBoard::graphColoredFully() {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {  
            if(cells_[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}
void SudokuBoard::reset() {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            cells_[i][j] = beforeSolve_[i][j];
        }
    }
    populateVisitingOrder();
}
bool SudokuBoard::solveGreedy() {

    for(int i = 0; i < 300; i++) {
        Cell start = visiting_order_.front();
        visiting_order_.pop();
        recursiveGraphColor(start);

        if(graphColoredFully()) {
            break;
        }
        reset();
    }
    return graphColoredFully();
}

bool SudokuBoard::solveBacktracking(std::vector<std::vector<int> >& adjMat, std::vector<std::vector<int> >& board) {
    //base case if board is full of non zero values
    if(graphColoredFully()) {
        return true;
    }
    //find the next cell to color
    bool valid = true;
    std::vector<int> availableColors = {1,2,3,4,5,6,7,8,9};
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            for(unsigned c = 0; c < availableColors.size(); c++) {
                if(board[i][j] != 0) {
                    continue;
                }
                valid = true;
                for(int k = 0; k < 81; k++) {
                    if(adjMat[9*i+j][k] == 1 && board[k/9][k%9] == availableColors[c]) {
                        valid = false;
                        break;
                    }
                }
                if(valid) {
                    board[i][j] = availableColors[c];
                    if(solveBacktracking(adjMat, board)) {
                        return true;
                    }
                    availableColors.erase(std::remove(availableColors.begin(), availableColors.end(), board[i][j]), availableColors.end());
                    board[i][j] = 0;
                    c = 0;
                }
            }
            if(!valid) {
                if(board[i][j] == 0) {
                    return false;
                }
            }
        }
    }
    return false;
}

std::string SudokuBoard::DFS() { //displays coordinates of the board before being solved;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (beforeSolve_[i][j] != 0 && visited_[i][j] == false) {
                // beforeSolve_ is the layout of the quiz board;
                visited_[i][j] = true;
                res += ("(" + std::to_string(i) + ", " + std::to_string(j) + ") ");
                DFS();
            }
        }
    }
    return res;
}

/////////////////////////////////////////////////////////////

std::vector<std::string> get_substrs(const std::string& str, char delimiter) {
  size_t last = 0;
  std::vector<std::string> substrs;
  for (size_t i = 0; i != str.length(); ++i) {
    if (str.at(i) == delimiter) {
      std::string substr = str.substr(last, i - last);
      last = i + 1;
      substrs.push_back(substr);
    }
  }
  std::string substr = str.substr(last, str.length() - last);
  substrs.push_back(substr);

  return substrs;
}

std::string removeNonNumber(std::string line) {
    std::string result;
    for (unsigned int i = 0; i < line.length(); i++) {
        if (isdigit(line[i]) == true) {
            result += line[i];
        }
    }
    return result;
}

//using this for tests in test.cpp
std::string SudokuBoard::toString() {
    std::string res = "";
    for (unsigned int i = 0; i < 9; i++) {
        for (unsigned int j = 0; j < 9; j++) {
            res += std::to_string(cells_[i][j]);
        }
    }
    return res;
}