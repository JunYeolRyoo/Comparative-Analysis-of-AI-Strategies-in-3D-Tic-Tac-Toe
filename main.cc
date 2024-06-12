#include "utility.cpp"
#define endl '\n'

using namespace std;

int main() {
    srand(time(NULL)); //Force rand() to produce pseudo-random numbers non-deterministically
    int playedGame = 0;
    int howmanyX, howmanyO, howmanyD = 0;
    
    /*
    The current setting is configured to play 5 games with two playing agents (MCTS with 'O' and Minimax with 'X').
    The maximum search time limit for the MCTS agent is set to 15 seconds, and the maximum search depth for the Minimax agent is set to a depth of 2. 
    You can manually adjust these settings by referring to the functions play_the_game_mcts() and play_the_game_minimax() in utility.cpp.
    */
    while(playedGame < 5){   // Adjust the number of game to play
        vector<vector<vector<char>>> ttt(4, vector<vector<char>> (4, vector<char> (4, '#')));
        play_the_game_mcts(ttt, true, 2);
        playedGame += 1;
        if((playerWinFast(ttt, 'O'))){
            howmanyO += 1;
            cout << "MCTS Won!" << endl;       // MCTS is an agent with piece X
        }else if(available_moves(ttt).size() == 0){
            howmanyD += 1;
            cout << "The Game Ended in a Draw!" << endl;
        }
        else{
            howmanyX += 1;
            cout << "Minimax Won!" << endl;       // Minimax is an agent with piece O
        }

        printTicTacToe(ttt);   
    }

    cout << "Total time spent for playing " << playedGame<<" games: " << timeusedO + timeusedX << endl;
    cout << "Total time used by MCTS: " << timeusedO << endl;      
    cout << "Total time used by Minimax: " << timeusedX << endl << endl;      

    cout << "MCTS Won: " << howmanyO << endl;
    cout << "Minimax Won: " << howmanyX << endl;
    cout << "Draw: " << howmanyD << endl;

    return 0;
}