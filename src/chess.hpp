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
    int * parseMove(std::string move);
    void urMove(std::string nextEngineMove);
    bool moveIsKill(std::string fen, int movex, int movey);
public:
    Chess(string path, Modbus* ur, Modbus* at);
    ~Chess();
    void userMove(std::string move);
    bool isGameOver();
    const void ping(Modbus *ur);
};