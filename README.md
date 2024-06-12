# Comparative-Analysis-of-AI-Strategies-in-3D-Tic-Tac-Toe
This project involves the implementation and analysis of various algorithms for developing an artificial intelligence (AI) agent for the complex game of 3D tic-tac-toe (with 4x4x4 grids). The primary focus is on two main algorithms: Monte Carlo Tree Search (MCTS) and Minimax with Alpha-Beta Pruning. This project includes comparative evaluations of these algorithms to determine the most effective approach for the 3D tic-tac-toe game.<br>

## Problem Overview
3D tic-tac-toe is a two-player board game similar to the traditional tic-tac-toe but played on a 4x4x4 grid, making it significantly more complex. The objective is to create a winning line of four consecutive pieces either horizontally, vertically, or diagonally across the 3D board. This increased complexity presents a challenging problem for developing an AI agent due to the vast number of possible game states.

3D Tic-Tac-Toe Board (Source: [Codewars](https://www.codewars.com/kata/5aa67541373c2e69a20000c9))<br>
![3D Tic-Tac-Toe Board](https://i.imgur.com/NlG3wai.png)

## Experiment Design
Several experiments were conducted to evaluate the performance of the AI agents:
  - Comparison of Monte Carlo Tree Search with different simulation times (e.g., 10 seconds, 15 seconds, etc.).
  - Performance comparison between MCTS and Minimax with Alpha-Beta Pruning.
  - Analysis of the impact of different evaluation functions on the Minimax algorithm.
  - Examination of the role of maximum search depth in the effectiveness of Alpha-Beta Pruning (search depths ranging from 1 to 7).

## Results and Analysis
The experiments showed the following:
  - The effectiveness of MCTS is influenced by the randomness of the expansion process and the simulation time.
  - Minimax with Alpha-Beta Pruning performs well when equipped with a high-quality evaluation function, almost always outperforming MCTS.
  - The choice of evaluation function and maximum search depth directly affects the performance of the Minimax algorithm.<br>

You can find more detailed comparisons and analysis in the [research paper](paper/Final_Report.pdf).

## Usage
```
# Navigate to the project directory 
cd /path/to/repo/project

# Build the project
g++ main.cc -o program_name

# Run the project
./program_name
```

## Conclusion
The project highlights the importance of selecting appropriate algorithms and evaluation functions for developing AI agents for complex games like 3D tic-tac-toe. While MCTS is less dependent on evaluation functions, Minimax with Alpha-Beta Pruning can achieve better results with a well-designed evaluation function for this specific game. 

## Acknowledgements
This project is based on code from [Original Author's Repository](https://github.com/diamantis-rafail-papadam/3D-Tic-Tac-Toe/tree/main/files). Significant changes have been made to tailor the implementation and adapt it for the specific needs of this project. The [research paper](paper/Final_Report.pdf) used as a reference for this project is:

> Jun Yeol Ryoo, University of Minnesota.
