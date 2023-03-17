#include "chess.hpp"
#include <stdio.h>
#include <iostream>
using namespace std;
int main()
{
    #if defined(__linux__) // Or #if __linux__
        Chess game("../stockfish/stockfish-ubuntu-20.04-x86-64","127.0.0.1","/dev/tty.usbserial-DK0AI63M",38400);
    #elif _WIN32
        cout << "Won't work on windows" << endl;
        return 1;
    #else
        Chess game("../Stockfish-master/src/stockfish", "127.0.0.1", "/dev/tty.usbserial-DK0AI63M", 38400);
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