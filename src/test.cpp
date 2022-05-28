#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "SudokuBoard.h"
#include <string>
#include <iostream>

// Sudoku Puzzle solving algorithm tests;

TEST_CASE( "Puzzle 1, Line 2", "[Test 1]") {
    SudokuBoard board = SudokuBoard("dataset.csv", 98);
    board.solveGreedy(board.getAdjacency(), board.getCells());
    std::string quiz = board.toString();
    std::string correctQuiz = "943265718267148395815397426731586942459712863682934571196873254324659187578421639";
    REQUIRE(quiz == correctQuiz);
}

TEST_CASE("All Puzzles", "[Test 2]") {
    SudokuBoard board("dataset.csv", 1);
    int counter = 0;
    int numberOfPuzzles = 100;
    for (int n = 1; n <= numberOfPuzzles; n++) {
        board = SudokuBoard("dataset.csv", n);
        if (board.solveGreedy(board.getAdjacency(), board.getCells())) {
            counter++;
        }
    }
    REQUIRE(counter == numberOfPuzzles);
}

// Data cleaning/parsing functionality tests;
// parseBoardToVector gets called in the SudokuBoard constructor;

TEST_CASE("parseBoardToVector works with whitespaces in the data", "[Test 1]") {
    SudokuBoard board = SudokuBoard("datasetTEST.csv", 1);
    std::string quiz = board.toString();
    std::string correctQuiz = "004300209005009001070060043006002087190007400050083000600000105003508690042910300";
    REQUIRE(quiz == correctQuiz);
}

TEST_CASE("parseBoardToVector works with non-digit characters in the data", "[Test 2]") {
    SudokuBoard board = SudokuBoard("datasetTEST.csv", 4);
    std::string quiz = board.toString();
    std::string correctQuiz = "497200000100400005000016098620300040300900000001072600002005870000600004530097061";
    REQUIRE(quiz == correctQuiz);
}

TEST_CASE("parseBoardToVector works with non-digit characters AND whitespaces in the data", "[Test 3]") {
    SudokuBoard board = SudokuBoard("datasetTEST.csv", 5);
    std::string quiz = board.toString();
    std::string correctQuiz = "005910308009403060027500100030000201000820007006007004000080000640150700890000420";
    REQUIRE(quiz == correctQuiz);
}

TEST_CASE("parseBoardToVector throws error when unsolved puzzle has less than required puzzle size", "[Test 4") {
    // Requires that an error is throw due to invalid board size;
    REQUIRE_THROWS_AS(SudokuBoard("datasetTEST.csv", 3), std::logic_error);
}

TEST_CASE("parseBoardToVector throws error when unsolved puzzle has more than required puzzle size", "[Test 5") {
    REQUIRE_THROWS_AS(SudokuBoard("datasetTEST.csv", 2), std::logic_error);
}

// DFS graph traversal;
// Displays coordinates of all non-zero digits of unsolved board;
TEST_CASE("DFS coordinates are correct", "[Test 1]") {
    SudokuBoard board("dataset.csv", 1);
    std::string res = board.DFS();
    std::cout << res << '\n';
}

//