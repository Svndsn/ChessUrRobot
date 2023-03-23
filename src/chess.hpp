#pragma once
#include "modbus.hpp"
#include "ChessEngine.h"
#include <string>
class Chess
{
private:
    ChessEngine sf;
    Modbus ur;
 
    std::string nextEngineMove;
    int * parseMove(std::string coordinate);
public:
    Chess(string path, Modbus* in);
    void urMove();
    void userMove(std::string coordinates);
    bool isGameOver();
    bool moveIsKill(std::string fen, int movex, int movey);
};