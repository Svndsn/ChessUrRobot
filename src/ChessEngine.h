#pragma once
#include <string>

using namespace std;

struct process {
    int pid;
    int infd;
    int outfd;
};

class ChessEngine {
private:
    process* engineProcess;
    string turn = "white";
    bool gameover = false;
    string board = "position startpos moves";

public:
    ChessEngine();
    ~ChessEngine();
    void startEngine(string fileName);
    void sendCommand(string command);
    string readResponse();
    void makeMove(string next_move);
    string getUserMove();
    void changeTurn();
    void endProcess();
    bool readGameover();
    string readTurn();
};
