#include "chess.hpp"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>
#include "database.h"
#include "ChessMoveDetector.h"
using namespace std;
//using namespace chrono_literals;
//using namespace this_thread;

void ping(Modbus *ur){
    while(true){
        ur->write(1,1);
        sleep(5);
    }
}
void ping2(Modbus *ur){
    while(true){
        ur->write(1002,1);
        sleep(5);
    }
}
int main()
{
    Modbus *ur = new Modbus("192.168.100.11");
    Modbus *at = new Modbus("/dev/ttyUSB0",10);
    thread pingThread(ping,ur);
    thread pingThread2(ping2,at);
    string name;
    cout << "Enter your name: " << endl;
    getline(cin,name);
    ChessRobotDatabase *db = new ChessRobotDatabase("root", "password");
    ChessMoveDetector *cam = new ChessMoveDetector(210,80,380,380); 
    #if defined(__linux__) // Or #if __linux__
        Chess game("../stockfish/stockfish-ubuntu-20.04-x86-64",ur,at,db,name,cam);
    #elif _WIN32
        cout << "Won't work on windows" << endl;
        return 1;
    #else
        Chess game("../Stockfish-master/src/stockfish", ur,at,db,name,cam);
    #endif
    
    while (!game.isGameOver())
    {
        game.getUserMove();
    }
    string whoWon;
    cout << "Who won?" << endl;
    cout << "'1' = White, '0' = Black, '-' = Draw" << endl;
    getline(cin,whoWon);
    db->win(whoWon);
    return 0;
}
  