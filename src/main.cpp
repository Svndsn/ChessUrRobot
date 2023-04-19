#include "chess.hpp"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>
#include "database.h"
using namespace std;
//using namespace chrono_literals;
//using namespace this_thread;

void ping(Modbus *ur){
    while(true){
        ur->write(1,1);
        sleep(5);
    }
}
int main()
{   
    Modbus *ur = new Modbus("192.168.100.11");
    Modbus *at = new Modbus("/dev/tty.usbserial-DK0AI3MN",10);
    ChessRobotDatabase db = ChessRobotDatabase("root", "password");
    #if defined(__linux__) // Or #if __linux__
        Chess game("../stockfish/stockfish-ubuntu-20.04-x86-64",ur,at);
    #elif _WIN32
        cout << "Won't work on windows" << endl;
        return 1;
    #else
        Chess game("../Stockfish-master/src/stockfish", ur,at);
    #endif
    thread pingThread(ping,ur);
    string name;
    cout << "Enter your name: " << endl;
    getline(cin,name);
    db.newGame(name);
    while (!game.isGameOver())
    {
        string move;
        cout << "Enter your move: " << endl;
        getline(cin, move);
        game.userMove(move);
}
