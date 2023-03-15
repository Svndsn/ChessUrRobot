#include "chess.hpp"
#include <stdio.h>
#include <iostream>
using namespace std;
int main()
{
    //Chess game("../Stockfish-master/src/stockfish","127.0.0.1","/dev/tty.usbserial-DK0AI63M",9600);
    Chess game("../stockfish/stockfish-ubuntu-20.04-x86-64","127.0.0.1","/dev/tty.usbserial-DK0AI63M",9600);

    while(1){
        string move;
        cout << "Enter your move: " << endl;
        getline(cin,move);
        game.userMove(move);




    }
}