#include "modbus.hpp"
#include "chess.hpp"
#include "ChessEngine.h"
#include <stdio.h>
#include <iostream>
using namespace std;
int main()
{
    
    Chess game("../stockfish/stockfish-ubuntu-20.04-x86-64","192.168.100.11");
    
    
    while(!game.isGameOver()){
        string move;
        cout << "Enter your move: " << endl;
        getline(cin,move);
        game.userMove(move);
        game.urMove();
        
        

        
    }
}