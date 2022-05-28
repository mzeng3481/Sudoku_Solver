# Results
## Output and Correctness
* Greedy Algorithm:
    * Uses a predetermined node visiting-order based on a saturation degree heuristic
    * Saturation degree is the number of distinct colors adjacent to a given node, and is relevant for visiting order because we intuitively want to color cells with the least options first
    * There is a high amount of ambiguity in visiting order even with this heuristic because there are many ties in most Sudoku boards, so determining which should be colored first is impossible to know with this heuristic alone
    * Thus, we use a component of randomness to make n number of attempts(n=300 in our code) with random permutations of the tied saturation degree nodes
    * Tested against multiple known solutions from the dataset to confirm accuracy and solves ~60% of all boards

Example run:
```
------- INIT BOARD ------
0  0  4  3  0  0  2  0  9  
0  0  5  0  0  9  0  0  1  
0  7  0  0  6  0  0  4  3  
0  0  6  0  0  2  0  8  7  
1  9  0  0  0  7  4  0  0  
0  5  0  0  8  3  0  0  0  
6  0  0  0  0  0  1  0  5  
0  0  3  5  0  8  6  9  0  
0  4  2  9  1  0  3  0  0  
------- INIT BOARD ------

------- MID BOARD -------
8  0  4  3  7  1  2  0  9  
0  0  5  0  0  9  7  6  1  
0  7  0  0  6  5  8  4  3  
4  3  6  1  9  2  5  8  7  
1  9  8  6  5  7  4  0  2  
2  5  7  0  8  3  9  0  6  
6  8  0  0  0  4  1  2  5  
7  1  3  5  0  8  6  9  4  
0  4  2  9  1  6  3  7  8  
------- MID BOARD -------

------ FINAL BOARD ------
8  6  4  3  7  1  2  5  9  
3  2  5  8  4  9  7  6  1  
9  7  1  2  6  5  8  4  3  
4  3  6  1  9  2  5  8  7  
1  9  8  6  5  7  4  3  2  
2  5  7  4  8  3  9  1  6  
6  8  9  7  3  4  1  2  5  
7  1  3  5  2  8  6  9  4  
5  4  2  9  1  6  3  7  8  
------ FINAL BOARD ------
```
* Backtracking Algorithm:
    * The backtracking algorithm is based around backtracking and attempts to place as many 1s,2s,3s,etc. as it can according
    to the adjacency matrix beginning in the top left and working top to bottom until the board is filled
    * Tested against multiple known solutions from the dataset to confirm accuracy and solves all 10000 boards

Example run:
```
------- INIT BOARD ------
0  0  4  3  0  0  2  0  9  
0  0  5  0  0  9  0  0  1  
0  7  0  0  6  0  0  4  3  
0  0  6  0  0  2  0  8  7  
1  9  0  0  0  7  4  0  0  
0  5  0  0  8  3  0  0  0  
6  0  0  0  0  0  1  0  5  
0  0  3  5  0  8  6  9  0  
0  4  2  9  1  0  3  0  0  
------- INIT BOARD ------

------- MID BOARD -------
8  6  4  3  7  1  2  5  9  
3  2  5  8  4  9  7  6  1  
9  7  1  2  6  5  8  4  3  
4  3  6  1  9  2  5  8  7  
1  9  0  0  0  7  4  0  0  
0  5  0  0  8  3  0  0  0  
6  0  0  0  0  0  1  0  5  
0  0  3  5  0  8  6  9  0  
0  4  2  9  1  0  3  0  0  
------- MID BOARD -------

------ FINAL BOARD ------
8  6  4  3  7  1  2  5  9  
3  2  5  8  4  9  7  6  1  
9  7  1  2  6  5  8  4  3  
4  3  6  1  9  2  5  8  7  
1  9  8  6  5  7  4  3  2  
2  5  7  4  8  3  9  1  6  
6  8  9  7  3  4  1  2  5  
7  1  3  5  2  8  6  9  4  
5  4  2  9  1  6  3  7  8  
------ FINAL BOARD ------
```

# How did we answer the leading question?
&emsp;&ensp;Obtaining a large and accurate dataset with unsolved and corresponding solved puzzles was vital.
Once we completed our greedy algorithm, the accuracy was not as good as we hoped so we attemped
the backtracking algorithm from class which worked phenomenally.

&emsp;&ensp;Our initial plan had also been to write a k-core algorithm, but after delving into the graph representation we discovered that we were working with a regular graph, all vertices had the same degree, resulting in an incredibly uninteresting graph. From there, we instead decided to improve the accuracy of our solving algorithm using a backtracking algorithm.
&emsp;&ensp;At first our algorithm wasn't great, but after a lot more debugging we discovered certain edge cases. For example, when backtracking the current cell is reset to "0", but if the current cell was revisited later, it would come across the same color and get stuck in an infinite loop. We solved this issue by adding an array of available colors to each cell that would update before each backtrack.

&emsp;&ensp;Although one algorithm wasn't perfect, by attacking the problem from a different perspective we were ultimately able to solve our leading question. In the future, we definitely learned that a lot of research and preparation should go into the leading question and proposal in order to reduce confusion and "backtracking" during execution.

