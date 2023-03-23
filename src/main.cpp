#include "chess.hpp"
#include <stdio.h>
#include <iostream>
using namespace std;
int main()
{
    Modbus *ur = new Modbus("192.168.100.100");
    #if defined(__linux__) // Or #if __linux__
        Chess game("../stockfish/stockfish-ubuntu-20.04-x86-64",ur);
    #elif _WIN32
        cout << "Won't work on windows" << endl;
        return 1;
    #else
        Chess game("../Stockfish-master/src/stockfish", ur);
    #endif

    while (1)
    {
        string move;
        cout << "Enter your move: " << endl;
        getline(cin, move);
        game.userMove(move);
        game.urMove();
    }
    


}
