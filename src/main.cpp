#include "chess.hpp"
#include <stdio.h>
#include <iostream>
using namespace std;
int main()
{
    ChessEngine sf;
    sf.startEngine("../Stockfish-master/src/stockfish");
    sf.readResponse();
    sf.sendCommand("isready");
    sf.readResponse();
    cout << "dit træk: " << endl;
    string move;
    getline(cin,move);
    sf.makeMove(move);
    cout << "poopoo" << endl;
    sf.sendCommand("go movetime 1000");
    while (sf.readResponse()!="")
    {
        sf.readResponse();
    }
    
    cout << "jeg er her" << endl;
    /*Chess game("../Stockfish-master/src/stockfish","127.0.0.1","/dev/tty.usbserial-DK0AI63M",9600);//"../stockfish/stockfish-ubuntu-20.04-x86-64"
    
    
    while(!game.isGameOver()){
        string move;
        cout << "Enter your move: " << endl;
        getline(cin,move);
        game.userMove(move);
        game.urMove();
        
        

        
    }
    */
}