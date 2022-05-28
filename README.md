# Locations
* ## /src
    * Code: main.cpp; SudokuBoard.cpp; SudokuBoard.h
    * Tests: test.cpp
    * Data: dataset.csv; datasetTEST.csv(for test.cpp)

# Setup
To run the greedy algorithm:
```
cd src
g++ -Wall -Werror -std=c++11 main.cpp SudokuBoard.cpp SudokuBoard.h -o sudoku
./sudoku dataset.csv greedy
```

To run the backtracking algorithm:
```
cd src
g++ -Wall -Werror -std=c++11 main.cpp SudokuBoard.cpp SudokuBoard.h -o sudoku
./sudoku dataset.csv backtracking
```

To run tests:
```
cd src
g++ -Wall -Werror -std=c++11 SudokuBoard.cpp test.cpp -o test
./test
```
* tests checks for:
    * correct board size
    * program's solution against known solution
    * data cleaning and parsing functionality


