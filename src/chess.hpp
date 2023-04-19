#pragma once
#include "modbus.hpp"
#include "ChessEngine.h"
#include <string>
#include "database.h"
class Chess
{
private:
    ChessEngine sf;
    Modbus ur;
    Modbus at;
    ChessRobotDatabase db;
    int * parseMove(std::string move);
    void urMove(std::string nextEngineMove);
    bool moveIsKill(std::string fen, int movex, int movey);
    std::string playerName;
public:
    Chess(string path, Modbus* ur, Modbus* at, ChessRobotDatabase *db, std::string name);
    ~Chess();
    void userMove(std::string move);
    bool isGameOver();
    const void ping(Modbus *ur);
};