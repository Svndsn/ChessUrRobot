#pragma once
#include "modbus.hpp"
#include "ChessEngine.h"
#include <string>
class Chess
{
private:
    ChessEngine sf;
    Modbus ur;
    Modbus at;

    int * parseMove(std::string coordinate);
public:
    Chess(string path, const char* ip,const char* device,int baud);
    void urMove();
    void userMove(std::string coordinates);
    bool isGameOver();
    bool moveIsKill(std::string fen, int movex, int movey);
};