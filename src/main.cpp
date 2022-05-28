
#include "SudokuBoard.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <stdexcept> 
#include <set>
#include <stdio.h>
#include <ctype.h>
#include <algorithm>
#include <chrono>
#include <random>
#include <cstring>

int main(int argc, char** argv) {

    SudokuBoard board(argv[1], 1);

    if(strcmp(argv[2], "greedy") == 0) {
        int counter = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for(int n = 1; n <= 1000; n++) {
            board = SudokuBoard(argv[1], n);
            if(board.solveGreedy()) {
                board.displayBoard();
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                counter++;
                std::cout << "Solved " << counter << "/1000 in " << duration.count() << "ms" << std::endl;
            } else {
                std::cout << "Failed to solve " << std::endl;
            }
        }
    } else if(strcmp(argv[2], "backtracking") == 0) {
        int counter = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for(int n = 1; n <= 1000; n++) {
            board = SudokuBoard(argv[1], n);
            //board.displayAdjacency();
            if(board.solveBacktracking(board.getAdjacency(),board.getCells())) {
                board.displayBoard();
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
                counter++;
                std::cout << "Solved " << counter << "/1000 in " << duration.count() << "ms" << std::endl;
            } else {
                std::cout << "Failed to solve " << std::endl;
            }        
        }
    }

    //board.DFS();
    return 0;
}