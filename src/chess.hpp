#pragma once
#include "modbus.hpp"
#include "ChessEngine.h"
#include <string>
class Chess
{
private:
    ChessEngine sf;
    Modbus ur;

    int * parseMove(std::string coordinate);
public:
    Chess(string path, const char* ip);
    void urMove();
    void userMove(std::string coordinates);
    bool isGameOver();
};