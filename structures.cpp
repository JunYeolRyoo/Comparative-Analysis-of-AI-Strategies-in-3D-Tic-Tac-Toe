#include <bits/stdc++.h>

using namespace std;

enum opponent {
    preschool_child,
    diamantis,
    professor_lagoudakis
};

vector<tuple<int, int, int>> theoritical_moves = { make_tuple(1, 1, 1), make_tuple(1, 1, 2), make_tuple(1, 2, 1), make_tuple(1, 2, 2), 
                                                   make_tuple(2, 1, 1), make_tuple(2, 1, 2), make_tuple(2, 2, 1), make_tuple(2, 2, 2) };

struct minimax_ret {
    tuple<int, int, int> move = make_tuple(-1, -1, -1); 
    int score = 0;
};

struct MCTS_node {
    vector<vector<vector<char>>> ttt;//= { { {'#', '#', '#', '#'}, {'#', '#', '#', '#'}, {'#', '#', '#', '#'}, {'#', '#', '#', '#'} }, { {'#', '#', '#', '#'}, {'#', '#', '#', '#'}, {'#', '#', '#', '#'}, {'#', '#', '#', '#'} }, { {'#', '#', '#', '#'}, {'#', '#', '#', '#'}, {'#', '#', '#', '#'}, {'#', '#', '#', '#'} }, { {'#', '#', '#', '#'}, {'#', '#', '#', '#'}, {'#', '#', '#', '#'}, {'#', '#', '#', '#'} } };
    tuple<int, int, int> move = make_tuple(-1, -1, -1);
    double w = 0;
    int n = 0;
    bool x_to_play;
    vector<MCTS_node> children;
    MCTS_node *parent;
};