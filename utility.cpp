#include <bits/stdc++.h>
#include "structures.cpp"

void play_the_game_mcts(vector<vector<vector<char>>> &ttt, bool user_x_player, int difficulty);
void play_the_game_minimax(vector<vector<vector<char>>> &ttt, bool user_x_player, int difficulty);

int MAX_DEPTH = 0; //set in "play_the_game_minimax" according to user choice.
double timeusedX = 0.0;
double timeusedO = 0.0;

using namespace std;

void printTicTacToe(vector<vector<vector<char>>> &ttt) {
    for(int layer = 0; layer < ttt.size(); layer++) {
        cout << "Layer " << layer+1 << ":" << endl;
        for(int i = 0; i < ttt[layer].size(); i++) {
            cout << "\t";
            for(int j = 0; j < ttt[layer][i].size(); j++) {
                cout << ttt[layer][i][j] << " ";
            }
            cout << endl;
        }
    }
}

void print_mcts_node(MCTS_node *node, int depth) {
    cout << "Depth: " << depth << endl;
}

void print_mcts_tree(MCTS_node *root, int depth) {
    print_mcts_node(root, depth);
    for(int i = 0; i < root->children.size(); i++) {
        print_mcts_tree(&(root->children[i]), depth+1);
    }
}

void initializeTicTacToe(vector<vector<vector<char>>> &ttt) {
    for(int layer = 0; layer < ttt.size(); layer++)
        for(int i = 0; i < ttt[layer].size(); i++)
            for(int j = 0; j < ttt[layer][i].size(); j++)
                ttt[layer][i][j] = '#';
}

bool playerWinLayer(vector<vector<vector<char>>> &ttt, char c) { //checks if player won with tic-tac-toe in some layer
    bool tic_tac_toe = false;
    for(int layer = 0; layer < ttt.size(); layer++) { 
        bool diag1 = true, diag2 = true;
        for(int i = 0; i < ttt[layer].size(); i++) {
            bool horizontal = true, vertical = true;
            //check diagonals in each layer
            diag1 = diag1 && ttt[layer][i][i] == c;
            diag2 = diag2 && ttt[layer][i][ttt[layer].size() - i - 1] == c;

            //check vertical and horizontal in each layer
            for(int j = 0; j < ttt[layer][0].size(); j++) {
                horizontal = horizontal && ttt[layer][i][j] == c;
                vertical = vertical && ttt[layer][j][i] == c;
            }
            tic_tac_toe = tic_tac_toe || horizontal || vertical;
        }
        tic_tac_toe = tic_tac_toe || diag1 || diag2;
    }
    return tic_tac_toe;
}

bool playerWinInterLayer(vector<vector<vector<char>>> &ttt, char c) { //check if player won with tic-tac-toe between different layers
    bool tic_tac_toe = false;
    
    for(int row = 0; row < ttt[0].size(); row++) {
        for(int col = 0; col < ttt[0][0].size(); col++) {
            bool curr_vertical = true;
            bool tmp1 = true, tmp2 = true, curr_one_diagonal = true, curr_two_diagonal = true;

            //check vertical ones
            for(int layer = 0; layer < ttt.size(); layer++)
                curr_vertical = curr_vertical && ttt[layer][row][col] == c;

            //check diagonal ones in one & two dimension
            if(row == 0 && col == 0) {
                curr_one_diagonal = false;
                for(int layer = 0; layer < ttt.size(); layer++) {
                    tmp1 = tmp1 && ttt[layer][row + layer][col] == c;
                    tmp2 = tmp2 && ttt[layer][row][col + layer] == c;
                    curr_two_diagonal = curr_two_diagonal && ttt[layer][row + layer][col + layer] == c;
                }
            }else if(row == 0 && col == 3) {
                curr_one_diagonal = false;
                for(int layer = 0; layer < ttt.size(); layer++) { 
                    tmp1 = tmp1 && ttt[layer][row + layer][col] == c;
                    tmp2 = tmp2 && ttt[layer][row][col - layer] == c;
                    curr_two_diagonal = curr_two_diagonal && ttt[layer][row + layer][col - layer] == c;
                }
            }else if(row == 3 && col == 0) {
                curr_one_diagonal = false;
                for(int layer = 0; layer < ttt.size(); layer++) {
                    tmp1 = tmp1 && ttt[layer][row - layer][col] == c;
                    tmp2 = tmp2 && ttt[layer][row][col + layer] == c;
                    curr_two_diagonal = curr_two_diagonal && ttt[layer][row - layer][col + layer] == c;
                }
            }else if(row == 3 && col == 3) {
                curr_one_diagonal = false;
                for(int layer = 0; layer < ttt.size(); layer++) {
                    tmp1 = tmp1 && ttt[layer][row - layer][col] == c;
                    tmp2 = tmp2 && ttt[layer][row][col - layer] == c;
                    curr_two_diagonal = curr_two_diagonal && ttt[layer][row - layer][col - layer] == c;
                }
            }else {
                tmp1 = false;
                tmp2 = false;
                curr_two_diagonal = false;
                if(row == 0)
                    for(int layer = 0; layer < ttt.size(); layer++)
                        curr_one_diagonal = curr_one_diagonal && ttt[layer][row + layer][col] == c;
                else if(row == 3)
                    for(int layer = 0; layer < ttt.size(); layer++)
                        curr_one_diagonal = curr_one_diagonal && ttt[layer][row - layer][col] == c;
                else if(col == 0)
                    for(int layer = 0; layer < ttt.size(); layer++)
                        curr_one_diagonal = curr_one_diagonal && ttt[layer][row][col + layer] == c;
                else if(col == 3)
                    for(int layer = 0; layer < ttt.size(); layer++)
                        curr_one_diagonal = curr_one_diagonal && ttt[layer][row][col - layer] == c;
                else
                    curr_one_diagonal = false;
            }
            curr_one_diagonal = curr_one_diagonal || tmp1 || tmp2;
            tic_tac_toe = tic_tac_toe || curr_vertical || curr_one_diagonal || curr_two_diagonal;
            //cout << row << " " << col << ": " << tic_tac_toe << endl;
        }
    }
    return tic_tac_toe;
}

bool playerWin(vector<vector<vector<char>>> &ttt, char c) { //checks if player won
    return playerWinLayer(ttt, c) || playerWinInterLayer(ttt, c);
}

bool playerWinFast(vector<vector<vector<char>>> &ttt, char c) { 
    bool tic_tac_toe = false;
    
    //in same layer
    tic_tac_toe = tic_tac_toe || (ttt[0][0][0] == c && ttt[0][0][1] == c && ttt[0][0][2] == c && ttt[0][0][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][1][0] == c && ttt[0][1][1] == c && ttt[0][1][2] == c && ttt[0][1][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][2][0] == c && ttt[0][2][1] == c && ttt[0][2][2] == c && ttt[0][2][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][0] == c && ttt[0][3][1] == c && ttt[0][3][2] == c && ttt[0][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][0] == c && ttt[0][1][0] == c && ttt[0][2][0] == c && ttt[0][3][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][1] == c && ttt[0][1][1] == c && ttt[0][2][1] == c && ttt[0][3][1] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][2] == c && ttt[0][1][2] == c && ttt[0][2][2] == c && ttt[0][3][2] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][3] == c && ttt[0][1][3] == c && ttt[0][2][3] == c && ttt[0][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][0] == c && ttt[0][1][1] == c && ttt[0][2][2] == c && ttt[0][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][3] == c && ttt[0][1][2] == c && ttt[0][2][1] == c && ttt[0][3][0] == c);

    tic_tac_toe = tic_tac_toe || (ttt[1][0][0] == c && ttt[1][0][1] == c && ttt[1][0][2] == c && ttt[1][0][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[1][1][0] == c && ttt[1][1][1] == c && ttt[1][1][2] == c && ttt[1][1][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[1][2][0] == c && ttt[1][2][1] == c && ttt[1][2][2] == c && ttt[1][2][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[1][3][0] == c && ttt[1][3][1] == c && ttt[1][3][2] == c && ttt[1][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[1][0][0] == c && ttt[1][1][0] == c && ttt[1][2][0] == c && ttt[1][3][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[1][0][1] == c && ttt[1][1][1] == c && ttt[1][2][1] == c && ttt[1][3][1] == c);
    tic_tac_toe = tic_tac_toe || (ttt[1][0][2] == c && ttt[1][1][2] == c && ttt[1][2][2] == c && ttt[1][3][2] == c);
    tic_tac_toe = tic_tac_toe || (ttt[1][0][3] == c && ttt[1][1][3] == c && ttt[1][2][3] == c && ttt[1][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[1][0][0] == c && ttt[1][1][1] == c && ttt[1][2][2] == c && ttt[1][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[1][0][3] == c && ttt[1][1][2] == c && ttt[1][2][1] == c && ttt[1][3][0] == c);

    tic_tac_toe = tic_tac_toe || (ttt[2][0][0] == c && ttt[2][0][1] == c && ttt[2][0][2] == c && ttt[2][0][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[2][1][0] == c && ttt[2][1][1] == c && ttt[2][1][2] == c && ttt[2][1][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[2][2][0] == c && ttt[2][2][1] == c && ttt[2][2][2] == c && ttt[2][2][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[2][3][0] == c && ttt[2][3][1] == c && ttt[2][3][2] == c && ttt[2][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[2][0][0] == c && ttt[2][1][0] == c && ttt[2][2][0] == c && ttt[2][3][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[2][0][1] == c && ttt[2][1][1] == c && ttt[2][2][1] == c && ttt[2][3][1] == c);
    tic_tac_toe = tic_tac_toe || (ttt[2][0][2] == c && ttt[2][1][2] == c && ttt[2][2][2] == c && ttt[2][3][2] == c);
    tic_tac_toe = tic_tac_toe || (ttt[2][0][3] == c && ttt[2][1][3] == c && ttt[2][2][3] == c && ttt[2][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[2][0][0] == c && ttt[2][1][1] == c && ttt[2][2][2] == c && ttt[2][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[2][0][3] == c && ttt[2][1][2] == c && ttt[2][2][1] == c && ttt[2][3][0] == c);

    tic_tac_toe = tic_tac_toe || (ttt[3][0][0] == c && ttt[3][0][1] == c && ttt[3][0][2] == c && ttt[3][0][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[3][1][0] == c && ttt[3][1][1] == c && ttt[3][1][2] == c && ttt[3][1][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[3][2][0] == c && ttt[3][2][1] == c && ttt[3][2][2] == c && ttt[3][2][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[3][3][0] == c && ttt[3][3][1] == c && ttt[3][3][2] == c && ttt[3][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[3][0][0] == c && ttt[3][1][0] == c && ttt[3][2][0] == c && ttt[3][3][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[3][0][1] == c && ttt[3][1][1] == c && ttt[3][2][1] == c && ttt[3][3][1] == c);
    tic_tac_toe = tic_tac_toe || (ttt[3][0][2] == c && ttt[3][1][2] == c && ttt[3][2][2] == c && ttt[3][3][2] == c);
    tic_tac_toe = tic_tac_toe || (ttt[3][0][3] == c && ttt[3][1][3] == c && ttt[3][2][3] == c && ttt[3][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[3][0][0] == c && ttt[3][1][1] == c && ttt[3][2][2] == c && ttt[3][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[3][0][3] == c && ttt[3][1][2] == c && ttt[3][2][1] == c && ttt[3][3][0] == c);

    //between different layers
    tic_tac_toe = tic_tac_toe || (ttt[0][0][0] == c && ttt[1][0][0] == c && ttt[2][0][0] == c && ttt[3][0][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][1] == c && ttt[1][0][1] == c && ttt[2][0][1] == c && ttt[3][0][1] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][2] == c && ttt[1][0][2] == c && ttt[2][0][2] == c && ttt[3][0][2] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][3] == c && ttt[1][0][3] == c && ttt[2][0][3] == c && ttt[3][0][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][1][0] == c && ttt[1][1][0] == c && ttt[2][1][0] == c && ttt[3][1][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][1][1] == c && ttt[1][1][1] == c && ttt[2][1][1] == c && ttt[3][1][1] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][1][2] == c && ttt[1][1][2] == c && ttt[2][1][2] == c && ttt[3][1][2] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][1][3] == c && ttt[1][1][3] == c && ttt[2][1][3] == c && ttt[3][1][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][2][0] == c && ttt[1][2][0] == c && ttt[2][2][0] == c && ttt[3][2][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][2][1] == c && ttt[1][2][1] == c && ttt[2][2][1] == c && ttt[3][2][1] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][2][2] == c && ttt[1][2][2] == c && ttt[2][2][2] == c && ttt[3][2][2] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][2][3] == c && ttt[1][2][3] == c && ttt[2][2][3] == c && ttt[3][2][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][0] == c && ttt[1][3][0] == c && ttt[2][3][0] == c && ttt[3][3][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][1] == c && ttt[1][3][1] == c && ttt[2][3][1] == c && ttt[3][3][1] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][2] == c && ttt[1][3][2] == c && ttt[2][3][2] == c && ttt[3][3][2] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][3] == c && ttt[1][3][3] == c && ttt[2][3][3] == c && ttt[3][3][3] == c);

    tic_tac_toe = tic_tac_toe || (ttt[0][0][0] == c && ttt[1][1][0] == c && ttt[2][2][0] == c && ttt[3][3][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][1] == c && ttt[1][1][1] == c && ttt[2][2][1] == c && ttt[3][3][1] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][2] == c && ttt[1][1][2] == c && ttt[2][2][2] == c && ttt[3][3][2] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][3] == c && ttt[1][1][3] == c && ttt[2][2][3] == c && ttt[3][3][3] == c);

    tic_tac_toe = tic_tac_toe || (ttt[0][0][3] == c && ttt[1][0][2] == c && ttt[2][0][1] == c && ttt[3][0][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][1][3] == c && ttt[1][1][2] == c && ttt[2][1][1] == c && ttt[3][1][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][2][3] == c && ttt[1][2][2] == c && ttt[2][2][1] == c && ttt[3][2][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][3] == c && ttt[1][3][2] == c && ttt[2][3][1] == c && ttt[3][3][0] == c);

    tic_tac_toe = tic_tac_toe || (ttt[0][3][3] == c && ttt[1][2][3] == c && ttt[2][1][3] == c && ttt[3][0][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][2] == c && ttt[1][2][2] == c && ttt[2][1][2] == c && ttt[3][0][2] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][1] == c && ttt[1][2][1] == c && ttt[2][1][1] == c && ttt[3][0][1] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][0] == c && ttt[1][2][0] == c && ttt[2][1][0] == c && ttt[3][0][0] == c);

    tic_tac_toe = tic_tac_toe || (ttt[0][3][0] == c && ttt[1][3][1] == c && ttt[2][3][2] == c && ttt[3][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][2][0] == c && ttt[1][2][1] == c && ttt[2][2][2] == c && ttt[3][2][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][1][0] == c && ttt[1][1][1] == c && ttt[2][1][2] == c && ttt[3][1][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][0] == c && ttt[1][0][1] == c && ttt[2][0][2] == c && ttt[3][0][3] == c);

    tic_tac_toe = tic_tac_toe || (ttt[0][0][0] == c && ttt[1][1][1] == c && ttt[2][2][2] == c && ttt[3][3][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][0][3] == c && ttt[1][1][2] == c && ttt[2][2][1] == c && ttt[3][3][0] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][0] == c && ttt[1][2][1] == c && ttt[2][1][2] == c && ttt[3][0][3] == c);
    tic_tac_toe = tic_tac_toe || (ttt[0][3][3] == c && ttt[1][2][2] == c && ttt[2][1][1] == c && ttt[3][0][0] == c);

    return tic_tac_toe; 
}

vector<tuple<int, int, int>> available_moves(vector<vector<vector<char>>> &ttt) {
    vector<tuple<int, int, int>> moves;
    for(int i = 0; i < ttt.size(); i++)
        for(int j = 0; j < ttt[i].size(); j++)
            for(int k = 0; k < ttt[i][j].size(); k++)
                if(ttt[i][j][k] == '#')
                    moves.push_back(make_tuple(i, j, k));
    return moves;
}

bool game_over(vector<vector<vector<char>>> &ttt) {
    return playerWinFast(ttt, 'X') || playerWinFast(ttt, 'O') || available_moves(ttt).size() == 0;
}

int evaluation(vector<vector<vector<char>>> &ttt) {
    int score = 0, x_score = 0, o_score = 0;

    //In any given layer
    for(int layer = 0; layer < ttt.size(); layer++) {
        int x_count_1 = 0, x_count_2 = 0, o_count_1 = 0, o_count_2 = 0;
        for(int i = 0; i < ttt[layer].size(); i++) {
           int x_count_layer_1 = 0, x_count_layer_2 = 0, o_count_layer_1 = 0, o_count_layer_2 = 0;
            //check diagonals in each layer
            x_count_1 += ttt[layer][i][i] == 'X';
            o_count_1 += ttt[layer][i][i] == 'O';

            x_count_2 += ttt[layer][i][ttt[layer].size() - i - 1] == 'X';
            o_count_2 += ttt[layer][i][ttt[layer].size() - i - 1] == 'O';

            //check vertical and horizontal in each layer
            for(int j = 0; j < ttt[layer][0].size(); j++) {
                x_count_layer_1 += ttt[layer][i][j] == 'X';
                o_count_layer_1 += ttt[layer][i][j] == 'O';

                x_count_layer_2 += ttt[layer][j][i] == 'X';
                o_count_layer_2 += ttt[layer][j][i] == 'O';
            }
            if(x_count_layer_1 == 4 || x_count_layer_2 == 4)
                return INT_MAX;
            if(o_count_layer_1 == 4 || o_count_layer_2 == 4)
                return INT_MIN;
            if(x_count_layer_1 == 3)
                x_score += 6;
            if(x_count_layer_2 == 3)
                x_score += 6;
            if(o_count_layer_1 == 3)
                o_score += 6;
            if(o_count_layer_2 == 3)
                o_score += 6;
            if(x_count_layer_1 == 2)
                x_score += 2;
            if(x_count_layer_2 == 2)
                x_score += 2;
            if(o_count_layer_1 == 2)
                o_score += 2;
            if(o_count_layer_2 == 2)
                o_score += 2;
        }
        if(x_count_1 == 4 || x_count_2 == 4)
            return INT_MAX;
        if(o_count_1 == 4 || o_count_2 == 4)
            return INT_MIN;
        if(x_count_1 == 3)
            x_score += 6;
        if(x_count_2 == 3)
            x_score += 6;
        if(o_count_1 == 3)
            o_score += 6;
        if(o_count_2 == 3)
            o_score += 6;
        if(x_count_1 == 2)
            x_score += 2;
        if(x_count_2 == 2)
            x_score += 2;
        if(o_count_1 == 2)
            o_score += 2;
        if(o_count_2 == 2)
            o_score += 2;
    }

    //Between different layers
    for(int row = 0; row < ttt[0].size(); row++) {
        for(int col = 0; col < ttt[0][0].size(); col++) {
            int x_count = 0, o_count = 0;
            int x1 = 0, x2 = 0, x3 = 0, o1 = 0, o2 = 0, o3 = 0;

            //check vertical ones
            for(int layer = 0; layer < ttt.size(); layer++) {
                x_count += ttt[layer][row][col] == 'X';
                o_count += ttt[layer][row][col] == 'O';
            }

            //check diagonal ones in one & two dimension
            if(row == 0 && col == 0) {
                for(int layer = 0; layer < ttt.size(); layer++) {
                    x1 += ttt[layer][row + layer][col] == 'X';
                    x2 += ttt[layer][row][col + layer] == 'X';
                    x3 += ttt[layer][row + layer][col + layer] == 'X';
                    o1 += ttt[layer][row + layer][col] == 'O';
                    o2 += ttt[layer][row][col + layer] == 'O';
                    o3 += ttt[layer][row + layer][col + layer] == 'O';
                }
            }else if(row == 0 && col == 3) {
                for(int layer = 0; layer < ttt.size(); layer++) { 
                    x1 += ttt[layer][row + layer][col] == 'X';
                    x2 += ttt[layer][row][col - layer] == 'X';
                    x3 += ttt[layer][row + layer][col - layer] == 'X';
                    o1 += ttt[layer][row + layer][col] == 'O';
                    o2 += ttt[layer][row][col - layer] == 'O';
                    o3 += ttt[layer][row + layer][col - layer] == 'O';
                }
            }else if(row == 3 && col == 0) {
                for(int layer = 0; layer < ttt.size(); layer++) {
                    x1 += ttt[layer][row - layer][col] == 'X';
                    x2 += ttt[layer][row][col + layer] == 'X';
                    x3 += ttt[layer][row - layer][col + layer] == 'X';
                    o1 += ttt[layer][row - layer][col] == 'O';
                    o2 += ttt[layer][row][col + layer] == 'O';
                    o3 += ttt[layer][row - layer][col + layer] == 'O';
                }
            }else if(row == 3 && col == 3) {
                for(int layer = 0; layer < ttt.size(); layer++) {
                    x1 += ttt[layer][row - layer][col] == 'X';
                    x2 += ttt[layer][row][col - layer] == 'X';
                    x3 += ttt[layer][row - layer][col - layer] == 'X';
                    o1 += ttt[layer][row - layer][col] == 'O';
                    o2 += ttt[layer][row][col - layer] == 'O';
                    o3 += ttt[layer][row - layer][col - layer] == 'O';
                }
            }else {
                if(row == 0)
                    for(int layer = 0; layer < ttt.size(); layer++) {
                        x1 += ttt[layer][row + layer][col] == 'X';
                        o1 += ttt[layer][row + layer][col] == 'O';
                    }
                else if(row == 3)
                    for(int layer = 0; layer < ttt.size(); layer++) {
                        x1 += ttt[layer][row - layer][col] == 'X';
                        o1 += ttt[layer][row - layer][col] == 'O';
                    }
                else if(col == 0)
                    for(int layer = 0; layer < ttt.size(); layer++) {
                        x1 += ttt[layer][row][col + layer] == 'X';
                        o1 += ttt[layer][row][col + layer] == 'O';
                    }
                else if(col == 3)
                    for(int layer = 0; layer < ttt.size(); layer++) {
                        x1 += ttt[layer][row][col - layer] == 'X';
                        o1 += ttt[layer][row][col - layer] == 'O';
                    }
            }
            if(x_count == 4 || x1 == 4 || x2 == 4 || x3 == 4)
                return INT_MAX;
            if(o_count == 4 || o1 == 4 || o2 == 4 || o3 == 4)
                return INT_MIN;
            if(x_count == 3)
                x_score += 6;
            if(x1 == 3)
                x_score += 6;
            if(x2 == 3)
                x_score += 6;
            if(x3 == 3)
                x_score += 6;
            if(o_count == 3)
                o_score += 6;
            if(o1 == 3)
                o_score += 6;
            if(o2 == 3)
                o_score += 6;
            if(o3 == 3)
                o_score += 6;
            if(x_count == 2)
                x_score += 2;
            if(x1 == 2)
                x_score += 2;
            if(x2 == 2)
                x_score += 2;
            if(x3 == 2)
                x_score += 2;
            if(o_count == 2)
                o_score += 2;
            if(o1 == 2)
                o_score += 2;
            if(o2 == 2)
                o_score += 2;
            if(o3 == 2)
                o_score += 2;   
        }
    }
    score = x_score - o_score;
    return score;
}

int evaluation2(vector<vector<vector<char>>> &ttt) {
    int score = 0, x_score = 0, o_score = 0;

    //In any given layer
    for(int layer = 0; layer < ttt.size(); layer++) {
        int x_count_1 = 0, x_count_2 = 0, o_count_1 = 0, o_count_2 = 0;
        for(int i = 0; i < ttt[layer].size(); i++) {
           int x_count_layer_1 = 0, x_count_layer_2 = 0, o_count_layer_1 = 0, o_count_layer_2 = 0;
            //check diagonals in each layer
            x_count_1 += ttt[layer][i][i] == 'X';
            o_count_1 += ttt[layer][i][i] == 'O';

            x_count_2 += ttt[layer][i][ttt[layer].size() - i - 1] == 'X';
            o_count_2 += ttt[layer][i][ttt[layer].size() - i - 1] == 'O';

            //check vertical and horizontal in each layer
            for(int j = 0; j < ttt[layer][0].size(); j++) {
                x_count_layer_1 += ttt[layer][i][j] == 'X';
                o_count_layer_1 += ttt[layer][i][j] == 'O';

                x_count_layer_2 += ttt[layer][j][i] == 'X';
                o_count_layer_2 += ttt[layer][j][i] == 'O';
            }
            if(x_count_layer_1 == 4 || x_count_layer_2 == 4)
                return INT_MAX;
            if(o_count_layer_1 == 4 || o_count_layer_2 == 4)
                return INT_MIN;
            if(x_count_layer_1 == 3 && o_count_layer_1 == 0)
                x_score += 6;
            if(x_count_layer_2 == 3 && o_count_layer_2 == 0)
                x_score += 6;
            if(o_count_layer_1 == 3 && x_count_layer_1 == 0)
                o_score += 6;
            if(o_count_layer_2 == 3 && x_count_layer_2 == 0)
                o_score += 6;
            if(x_count_layer_1 == 2 && o_count_layer_1 == 0)
                x_score += 2;
            if(x_count_layer_2 == 2 && o_count_layer_2 == 0)
                x_score += 2;
            if(o_count_layer_1 == 2 && x_count_layer_1 == 0)
                o_score += 2;
            if(o_count_layer_2 == 2 && x_count_layer_2 == 0)
                o_score += 2;
        }
        if(x_count_1 == 4 || x_count_2 == 4)
            return INT_MAX;
        if(o_count_1 == 4 || o_count_2 == 4)
            return INT_MIN;
        if(x_count_1 == 3 && o_count_1 == 0)
            x_score += 6;
        if(x_count_2 == 3 && o_count_2 == 0)
            x_score += 6;
        if(o_count_1 == 3 && x_count_1 == 0)
            o_score += 6;
        if(o_count_2 == 3 && x_count_2 == 0)
            o_score += 6;
        if(x_count_1 == 2 && o_count_1 == 0)
            x_score += 2;
        if(x_count_2 == 2 && o_count_2 == 0)
            x_score += 2;
        if(o_count_1 == 2 && x_count_1 == 0)
            o_score += 2;
        if(o_count_2 == 2 && x_count_2 == 0)
            o_score += 2;
    }

    //Between different layers
    for(int row = 0; row < ttt[0].size(); row++) {
        for(int col = 0; col < ttt[0][0].size(); col++) {
            int x_count = 0, o_count = 0;
            int x1 = 0, x2 = 0, x3 = 0, o1 = 0, o2 = 0, o3 = 0;

            //check vertical ones
            for(int layer = 0; layer < ttt.size(); layer++) {
                x_count += ttt[layer][row][col] == 'X';
                o_count += ttt[layer][row][col] == 'O';
            }

            //check diagonal ones in one & two dimension
            if(row == 0 && col == 0) {
                for(int layer = 0; layer < ttt.size(); layer++) {
                    x1 += ttt[layer][row + layer][col] == 'X';
                    x2 += ttt[layer][row][col + layer] == 'X';
                    x3 += ttt[layer][row + layer][col + layer] == 'X';
                    o1 += ttt[layer][row + layer][col] == 'O';
                    o2 += ttt[layer][row][col + layer] == 'O';
                    o3 += ttt[layer][row + layer][col + layer] == 'O';
                }
            }else if(row == 0 && col == 3) {
                for(int layer = 0; layer < ttt.size(); layer++) { 
                    x1 += ttt[layer][row + layer][col] == 'X';
                    x2 += ttt[layer][row][col - layer] == 'X';
                    x3 += ttt[layer][row + layer][col - layer] == 'X';
                    o1 += ttt[layer][row + layer][col] == 'O';
                    o2 += ttt[layer][row][col - layer] == 'O';
                    o3 += ttt[layer][row + layer][col - layer] == 'O';
                }
            }else if(row == 3 && col == 0) {
                for(int layer = 0; layer < ttt.size(); layer++) {
                    x1 += ttt[layer][row - layer][col] == 'X';
                    x2 += ttt[layer][row][col + layer] == 'X';
                    x3 += ttt[layer][row - layer][col + layer] == 'X';
                    o1 += ttt[layer][row - layer][col] == 'O';
                    o2 += ttt[layer][row][col + layer] == 'O';
                    o3 += ttt[layer][row - layer][col + layer] == 'O';
                }
            }else if(row == 3 && col == 3) {
                for(int layer = 0; layer < ttt.size(); layer++) {
                    x1 += ttt[layer][row - layer][col] == 'X';
                    x2 += ttt[layer][row][col - layer] == 'X';
                    x3 += ttt[layer][row - layer][col - layer] == 'X';
                    o1 += ttt[layer][row - layer][col] == 'O';
                    o2 += ttt[layer][row][col - layer] == 'O';
                    o3 += ttt[layer][row - layer][col - layer] == 'O';
                }
            }else {
                if(row == 0)
                    for(int layer = 0; layer < ttt.size(); layer++) {
                        x1 += ttt[layer][row + layer][col] == 'X';
                        o1 += ttt[layer][row + layer][col] == 'O';
                    }
                else if(row == 3)
                    for(int layer = 0; layer < ttt.size(); layer++) {
                        x1 += ttt[layer][row - layer][col] == 'X';
                        o1 += ttt[layer][row - layer][col] == 'O';
                    }
                else if(col == 0)
                    for(int layer = 0; layer < ttt.size(); layer++) {
                        x1 += ttt[layer][row][col + layer] == 'X';
                        o1 += ttt[layer][row][col + layer] == 'O';
                    }
                else if(col == 3)
                    for(int layer = 0; layer < ttt.size(); layer++) {
                        x1 += ttt[layer][row][col - layer] == 'X';
                        o1 += ttt[layer][row][col - layer] == 'O';
                    }
            }
            if(x_count == 4 || x1 == 4 || x2 == 4 || x3 == 4)
                return INT_MAX;
            if(o_count == 4 || o1 == 4 || o2 == 4 || o3 == 4)
                return INT_MIN;
            if(x_count == 3 && o_count == 0)
                x_score += 6;
            if(x1 == 3 && o1 == 0)
                x_score += 6;
            if(x2 == 3 && o2 == 0)
                x_score += 6;
            if(x3 == 3 && o3 == 0)
                x_score += 6;
            if(o_count == 3 && x_count == 0)
                o_score += 6;
            if(o1 == 3 && x1 == 0)
                o_score += 6;
            if(o2 == 3 && x2 == 0)
                o_score += 6;
            if(o3 == 3 && x3 == 0)
                o_score += 6;
            if(x_count == 2 && o_count == 0)
                x_score += 2;
            if(x1 == 2 && o1 == 0)
                x_score += 2;
            if(x2 == 2 && o2 == 0)
                x_score += 2;
            if(x3 == 2 && o3 == 0)
                x_score += 2;
            if(o_count == 2 && x_count == 0)
                o_score += 2;
            if(o1 == 2 && x1 == 0)
                o_score += 2;
            if(o2 == 2 && x2 == 0)
                o_score += 2;
            if(o3 == 2 && x3 == 0)
                o_score += 2;   
        }
    }
    score = x_score - o_score;
    return score;
}

bool make_move(vector<vector<vector<char>>> &ttt, int x1, int x2, int x3, char c) {
    if(x1 > 0 && x1 < 5 && x2 > 0 && x2 < 5 && x3 > 0 && x3 < 5 && ttt[x1 - 1][x2 - 1][x3 - 1] == '#') {
        ttt[x1 - 1][x2 - 1][x3 - 1] = c;
        return true;
    }else {
        return false;
    }
}

void make_player_move(vector<vector<vector<char>>> &ttt, char c){
    play_the_game_minimax(ttt,false,2);
}

bool compare_x_player(const pair<tuple<int, int, int>, int> &a, const pair<tuple<int, int, int>, int> &b) {
    return a.second > b.second;
}

bool compare_o_player(const pair<tuple<int, int, int>, int> &a, const pair<tuple<int, int, int>, int> &b) {
    return a.second < b.second;
}

vector<tuple<int, int, int>> sort_using_score(vector<tuple<int, int, int>> &moves, vector<int> &score, bool x_player) {
    vector<tuple<int, int, int>> sorted_moves;
    vector<pair<tuple<int, int, int>, int>> arr;
    for(int i = 0; i < moves.size(); i++)
        arr.push_back(make_pair(moves[i], score[i]));
    x_player ? sort(arr.begin(), arr.end(), compare_x_player) : sort(arr.begin(), arr.end(), compare_o_player);
    for(int i = 0; i < arr.size(); i++)
        sorted_moves.push_back(arr[i].first);
    return sorted_moves;
}

vector<tuple<int, int, int>> sort_moves(vector<vector<vector<char>>> &ttt, vector<tuple<int, int, int>> &moves, char c, int (*eval_func)(vector<vector<vector<char>>> &)) {
    vector<int> score;
    for(tuple<int, int, int> move : moves) {
        ttt[get<0>(move)][get<1>(move)][get<2>(move)] = c;
        score.push_back(eval_func(ttt));
        ttt[get<0>(move)][get<1>(move)][get<2>(move)] = '#';
    }
    return sort_using_score(moves, score, c == 'X');
}

int forward_pruning(int initial_size, int depth) {
    int moves_to_keep;
    if(depth < 3)
        moves_to_keep = initial_size;
    else
        moves_to_keep = min(max((int)round(64 - exp(depth)), 1), initial_size); //singular extensions are caused for depth=5 and higher
    return moves_to_keep;
}

tuple<int, int, int> get_theoritical_move(vector<vector<vector<char>>> &ttt, int move_size) { //In the beginning of the game our program plays according to theory.
    if(move_size > 59) {
        while(true) {
            int random = rand() % theoritical_moves.size();
            if(ttt[get<0>(theoritical_moves[random])][get<1>(theoritical_moves[random])][get<2>(theoritical_moves[random])] == '#')
                return theoritical_moves[random];
        }
    } else
        return make_tuple(-1, -1, -1);
}

tuple<int, int, int> random_move(vector<vector<vector<char>>> &ttt) {
    vector<tuple<int, int, int>> possible_moves = available_moves(ttt);
    int random = rand() % possible_moves.size();
    return possible_moves[random];
}

minimax_ret minimax(vector<vector<vector<char>>> &ttt, int depth, bool x_player, int (*eval_func)(vector<vector<vector<char>>> &)) { //X is the maximizing player
    minimax_ret ret;
    if(playerWinFast(ttt, 'X')) {
        ret.score = INT_MAX;
        return ret;
    }
    if(playerWinFast(ttt, 'O')) {
        ret.score = INT_MIN;
        return ret;
    }
    if(available_moves(ttt).size() == 0) {
        ret.score = 0;
        return ret;
    }
    if(depth == MAX_DEPTH) {
        ret.score = eval_func(ttt);
        return ret;
    }
    vector<tuple<int, int, int>> sorted_moves, possible_moves = available_moves(ttt);
    char c = x_player ? 'X' : 'O';
    if(x_player) {
        ret.score = INT_MIN;
        sorted_moves = sort_moves(ttt, possible_moves, 'X', eval_func);
        for(int i = 0; i < forward_pruning(sorted_moves.size(), depth); i++) {
            tuple<int, int, int> move = sorted_moves[i];
            ttt[get<0>(move)][get<1>(move)][get<2>(move)] = 'X';
            int eval = minimax(ttt, depth + 1, false, eval_func).score;
            if(eval > ret.score) {
                ret.score = eval;
                ret.move = move;
            }
            ttt[get<0>(move)][get<1>(move)][get<2>(move)] = '#';
        }
        return ret;
    }else {
        ret.score = INT_MAX;
        sorted_moves = sort_moves(ttt, possible_moves, 'O', eval_func);
        for(int i = 0; i < forward_pruning(sorted_moves.size(), depth); i++) {
            tuple<int, int, int> move = sorted_moves[i];
            ttt[get<0>(move)][get<1>(move)][get<2>(move)] = 'O';
            int eval = minimax(ttt, depth + 1, true, eval_func).score;
            if(eval < ret.score) {
                ret.score = eval;
                ret.move = move;
            }
            ttt[get<0>(move)][get<1>(move)][get<2>(move)] = '#';
        }
        return ret;
    }
}

minimax_ret minimax_ab_pruning(vector<vector<vector<char>>> &ttt, int depth, int alpha, int beta, bool x_player, int (*eval_func)(vector<vector<vector<char>>> &)) { //X is the maximizing player
    minimax_ret ret;
    if(playerWinFast(ttt, 'X')) {
        ret.score = INT_MAX;
        return ret;
    }
    if(playerWinFast(ttt, 'O')) {
        ret.score = INT_MIN;
        return ret;
    }
    if(available_moves(ttt).size() == 0) {
        ret.score = 0;
        return ret;
    }
    if(depth == MAX_DEPTH) {
        ret.score = eval_func(ttt);
        return ret;
    }
    vector<tuple<int, int, int>> sorted_moves, possible_moves = available_moves(ttt);
    char c = x_player ? 'X' : 'O';
    if(x_player) {
        ret.score = INT_MIN;
        sorted_moves = sort_moves(ttt, possible_moves, 'X', eval_func);
        for(int i = 0; i < forward_pruning(sorted_moves.size(), depth); i++) {
            tuple<int, int, int> move = sorted_moves[i];
            ttt[get<0>(move)][get<1>(move)][get<2>(move)] = 'X';
            int eval = minimax_ab_pruning(ttt, depth + 1, alpha, beta, false, eval_func).score;
            if(eval > ret.score) {
                ret.score = eval;
                ret.move = move;
            }
            ttt[get<0>(move)][get<1>(move)][get<2>(move)] = '#';

            alpha = max(alpha, eval);
            if(alpha >= beta)
                break;
        }
        return ret;
    }else {
        ret.score = INT_MAX;
        sorted_moves = sort_moves(ttt, possible_moves, 'O', eval_func);
        for(int i = 0; i < forward_pruning(sorted_moves.size(), depth); i++) {
            tuple<int, int, int> move = sorted_moves[i];
            ttt[get<0>(move)][get<1>(move)][get<2>(move)] = 'O';
            int eval = minimax_ab_pruning(ttt, depth + 1, alpha, beta, true, eval_func).score;
            if(eval < ret.score) {
                ret.score = eval;
                ret.move = move;
            }
            ttt[get<0>(move)][get<1>(move)][get<2>(move)] = '#';

            beta = min(beta, eval);
            if(alpha >= beta)
                break;
        }
        return ret;
    }
}

double UCB(MCTS_node &node, int parent_simulations) {
    double c = sqrt(2);
    if(node.n == 0)
        return DBL_MAX;
    else
        return (double)node.w / node.n + c*sqrt(log(parent_simulations)/node.n);
}

MCTS_node *mcts_select(MCTS_node *root) {
    if(root->children.size() == 0)
        return root;
    MCTS_node *best_node_to_expand;
    double max_ucb_value = 0;
    for(int i = 0; i < root->children.size(); i++) {
        double curr_ucb_value = UCB(root->children[i], root->n);
        if(curr_ucb_value > max_ucb_value) {
            best_node_to_expand = &(root->children[i]);
            max_ucb_value = curr_ucb_value;
        }
    }
    return mcts_select(best_node_to_expand);
}

MCTS_node *mcts_expand(MCTS_node *node_to_expand) {
    if(node_to_expand->n == 0)
        return node_to_expand;
    else {
        vector<tuple<int, int, int>> avail_moves = available_moves(node_to_expand->ttt);
        for(int i = 0; i < avail_moves.size(); i++) {
            MCTS_node child;
            child.move = avail_moves[i];
            child.n = 0;
            child.w = 0;
            child.parent = node_to_expand;
            child.x_to_play = !node_to_expand->x_to_play;
            child.ttt = node_to_expand->ttt;
            child.ttt[get<0>(avail_moves[i])][get<1>(avail_moves[i])][get<2>(avail_moves[i])] = node_to_expand->x_to_play ? 'X' : 'O';
            node_to_expand->children.push_back(child);
        }
        int random_child_index = rand() % node_to_expand->children.size();
        return &(node_to_expand->children[random_child_index]);
    }
}

int mcts_random_simulate(vector<vector<vector<char>>> ttt, bool x_to_play) {
    bool x_plays = x_to_play;
    while(!game_over(ttt)) {
        tuple<int, int, int> move = random_move(ttt);
        ttt[get<0>(move)][get<1>(move)][get<2>(move)] = x_plays ? 'X' : 'O';
        x_plays = !x_plays;
    }
    if(available_moves(ttt).size() == 0)
        return 0;
    else if(playerWinFast(ttt, 'X'))
        return 1;
    else if(playerWinFast(ttt, 'O'))
        return 2;
    else
        return -1; //impossible
}

void mcts_backpropagate(MCTS_node *root, MCTS_node *simulated_node, int simulation_result) {
    MCTS_node *curr_node = simulated_node;
    while(curr_node != root) {
        curr_node->n++;
        curr_node->w += (simulation_result == 1 && curr_node->x_to_play) || (simulation_result == 2 && !curr_node->x_to_play) ? 1 : 0;
        curr_node->w += simulation_result == 0 ? 0.5 : 0;
        curr_node = curr_node->parent;
    }
    root->n++;
    root->w += (simulation_result == 1 && root->x_to_play) || (simulation_result == 2 && !root->x_to_play) ? 1 : 0;
    root->w += simulation_result == 0 ? 0.5 : 0;
}

tuple<int, int, int> mcts_best_move(MCTS_node *root) {
    int most_simulations = 0;
    MCTS_node *best_child;
    for(int i = 0; i < root->children.size(); i++) {
        if(root->children[i].n > most_simulations) {
            most_simulations = root->children[i].n;
            best_child = &(root->children[i]);
        }
    }
    return best_child->move;
}

tuple<int, int, int> mcts_best_move2(MCTS_node *root) {
    int least_wins = INT_MAX;
    MCTS_node *best_child;
    for(int i = 0; i < root->children.size(); i++) {
        if(root->children[i].w  < least_wins) {
            least_wins = root->children[i].w;
            best_child = &(root->children[i]);
        }
    }
    return best_child->move;
}

tuple<int, int, int> mcts_best_move3(MCTS_node *root) {
    double lowest_win_ratio = DBL_MAX;
    MCTS_node *best_child;
    for(int i = 0; i < root->children.size(); i++) {
        double curr_win_ratio = (double)root->children[i].w/root->children[i].n;
        if(curr_win_ratio < lowest_win_ratio) {
            lowest_win_ratio = curr_win_ratio;
            best_child = &(root->children[i]);
        }
    }
    return best_child->move;
}

tuple<int, int, int> MCTS(vector<vector<vector<char>>> &ttt, double max_duration, bool x_to_play) {
    clock_t start = clock();
    double duration = 0;
    int counter = 0;
    
    MCTS_node root;
    root.ttt = ttt;
    root.x_to_play = x_to_play;
    root.w = 0;
    root.n = 0;
    MCTS_node *r = &root;
    while(duration < max_duration) { //max_duration is defined in seconds.
        MCTS_node *best_node_to_expand = mcts_select(r);
        MCTS_node *node_to_simulate = mcts_expand(best_node_to_expand);
        int simulation_result = mcts_random_simulate(node_to_simulate->ttt, node_to_simulate->x_to_play); //0 = draw, 1 = x wins, 2 = o wins.
        if(simulation_result == -1)
            cout << "It should be impossible for the program to reach this code!" << endl;
        mcts_backpropagate(r, node_to_simulate, simulation_result);

        counter++;
        duration = (double)(clock() - start)/CLOCKS_PER_SEC;
    }
    cout << "Iterations by MCTS: " << counter << endl;
    return mcts_best_move3(r);
}

void play_the_game_random(vector<vector<vector<char>>> &ttt, bool user_x_player) {
    tuple<int, int, int> computer;
    tuple<int, int, int> user_move;
    clock_t start, stop;
    double duration;
    char c = user_x_player ? 'X' : 'O';
    while(!game_over(ttt)) {
        if(user_x_player) {
            make_player_move(ttt, c);
            printTicTacToe(ttt);
            if(game_over(ttt))
                break;
            cout << "Computer Thinking... " << endl;
            start = clock();
            computer = random_move(ttt);
            cout << "Computer Played: " << get<0>(computer)+1 << " " << get<1>(computer)+1 << " " << get<2>(computer)+1 << endl;
            ttt[get<0>(computer)][get<1>(computer)][get<2>(computer)] = 'O';
            printTicTacToe(ttt);
            stop = clock();
            duration = (double)(clock() - start)/CLOCKS_PER_SEC;
            printf("Run Time: %.2f seconds\n", duration);
        }else {
            start = clock();
            cout << "Computer Thinking... " << endl;
            computer = random_move(ttt);
            cout << "Computer Played: " << get<0>(computer)+1 << " " << get<1>(computer)+1 << " " << get<2>(computer)+1 << endl;
            ttt[get<0>(computer)][get<1>(computer)][get<2>(computer)] = 'X';
            printTicTacToe(ttt);
            stop = clock();
            duration = (double)(clock() - start)/CLOCKS_PER_SEC;
            printf("Run Time: %.2f seconds\n", duration);
            if(game_over(ttt))
                break;
            make_player_move(ttt, c);
            printTicTacToe(ttt);
        }
    }
}

void play_the_game_mcts(vector<vector<vector<char>>> &ttt, bool user_x_player, int difficulty) {
    double mcts_duration;
    switch(difficulty) {
        case 1:
            mcts_duration = 10;
            break;
        case 2:
            mcts_duration = 15;
            break;
        case 3:
            mcts_duration = 50;
            break;
        default:
            cout << "It should be impossible for the program to reach this code!" << endl;
    };
    tuple<int, int, int> computer;
    tuple<int, int, int> user_move, computer_move;
    clock_t start, stop;
    double duration;
    char c = user_x_player ? 'X' : 'O';
    while(!game_over(ttt)) {
        if(user_x_player) {
            make_player_move(ttt, c);
            // printTicTacToe(ttt);
            if(game_over(ttt))
                break;
            cout << "Computer Thinking... " << endl;
            start = clock();
            tuple<int, int, int> theoritical_move = get_theoritical_move(ttt, available_moves(ttt).size());
            if(get<0>(theoritical_move) != -1 && get<1>(theoritical_move) != -1 && get<2>(theoritical_move) != -1)
                computer_move = theoritical_move;
            else {
                computer = MCTS(ttt, mcts_duration, false);
                computer_move = computer;
            }
            cout << "Computer Played: " << get<0>(computer_move)+1 << " " << get<1>(computer_move)+1 << " " << get<2>(computer_move)+1 << endl;
            ttt[get<0>(computer_move)][get<1>(computer_move)][get<2>(computer_move)] = 'O';
            // printTicTacToe(ttt);
            stop = clock();
            duration = (double)(clock() - start)/CLOCKS_PER_SEC;
            printf("Run Time: %.2f seconds\n", duration);
            timeusedO += duration;
        }else {
            cout << "Computer Thinking... " << endl;
            start = clock();
            tuple<int, int, int> theoritical_move = get_theoritical_move(ttt, available_moves(ttt).size());
            if(get<0>(theoritical_move) != -1 && get<1>(theoritical_move) != -1 && get<2>(theoritical_move) != -1)
                computer_move = theoritical_move;
            else {
                computer = MCTS(ttt, mcts_duration, true);
                computer_move = computer;
            }
            cout << "Computer Played: " << get<0>(computer_move)+1 << " " << get<1>(computer_move)+1 << " " << get<2>(computer_move)+1 << endl;
            ttt[get<0>(computer_move)][get<1>(computer_move)][get<2>(computer_move)] = 'X';
            // printTicTacToe(ttt);
            stop = clock();
            duration = (double)(clock() - start)/CLOCKS_PER_SEC;
            printf("Run Time: %.2f seconds\n", duration);
            if(game_over(ttt))
                break;
            // make_player_move(ttt, c);
            // printTicTacToe(ttt);
            break;
        }
    }
}

void play_the_game_minimax(vector<vector<vector<char>>> &ttt, bool user_x_player, int difficulty) {
    switch(difficulty) {
        case 1:
            MAX_DEPTH = 1;
            break;
        case 2:
            MAX_DEPTH = 2;
            break;
        case 3:
            MAX_DEPTH = 3;
            break;
        case 4:
            MAX_DEPTH = 4;
            break;
        case 5:
            MAX_DEPTH = 5;
            break;
        case 6:
            MAX_DEPTH = 6;
            break;
        case 7:
            MAX_DEPTH = 7;
            break;
        default:
            cout << "It should be impossible for the program to reach this code!" << endl;
    };
    minimax_ret computer;
    tuple<int, int, int> user_move, computer_move;
    clock_t start, stop;
    double duration;
    char c = user_x_player ? 'X' : 'O';
    while(!game_over(ttt)) {
        if(user_x_player) {
            make_player_move(ttt, c);
            // printTicTacToe(ttt);
            if(game_over(ttt))
                break;
            cout << "Computer Thinking... " << endl;
            start = clock();
            //computer = minimax(ttt, 0, false, evaluation2); //basic version of minimax which is not to be used.
            tuple<int, int, int> theoritical_move = get_theoritical_move(ttt, available_moves(ttt).size());
            if(get<0>(theoritical_move) != -1 && get<1>(theoritical_move) != -1 && get<2>(theoritical_move) != -1)
                computer_move = theoritical_move;
            else {
                computer = minimax_ab_pruning(ttt, 0, INT_MIN, INT_MAX, false, evaluation2);
                if(computer.score == INT_MAX)
                    computer.move = available_moves(ttt)[0];
                computer_move = computer.move;
            }
            cout << "Computer Played: " << get<0>(computer_move)+1 << " " << get<1>(computer_move)+1 << " " << get<2>(computer_move)+1 << endl;
            ttt[get<0>(computer_move)][get<1>(computer_move)][get<2>(computer_move)] = 'O';
            printTicTacToe(ttt);
            stop = clock();
            duration = (double)(clock() - start)/CLOCKS_PER_SEC;
            printf("Run Time: %.2f seconds\n", duration);
        }else {
            cout << "Computer Thinking... " << endl;
            start = clock();
            //computer = minimax(ttt, 0, true, evaluation2); //basic version of minimax which is not to be used.
            tuple<int, int, int> theoritical_move = get_theoritical_move(ttt, available_moves(ttt).size());
            if(get<0>(theoritical_move) != -1 && get<1>(theoritical_move) != -1 && get<2>(theoritical_move) != -1)
                computer_move = theoritical_move;
            else {
                computer = minimax_ab_pruning(ttt, 0, INT_MIN, INT_MAX, true, evaluation2);
                if(computer.score == INT_MIN)
                    computer.move = available_moves(ttt)[0];
                computer_move = computer.move;
            }
            cout << "Computer Played: " << get<0>(computer_move)+1 << " " << get<1>(computer_move)+1 << " " << get<2>(computer_move)+1 << endl;
            ttt[get<0>(computer_move)][get<1>(computer_move)][get<2>(computer_move)] = 'X';
            // printTicTacToe(ttt);
            stop = clock();
            duration = (double)(clock() - start)/CLOCKS_PER_SEC;
            printf("Run Time: %.2f seconds\n", duration);
            // if(game_over(ttt))
            timeusedX += duration;
            break;
            // make_player_move(ttt, c);
            // printTicTacToe(ttt);
        }
    }
}