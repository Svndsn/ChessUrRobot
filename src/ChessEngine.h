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
    bool gameover = false;
    string board = "position startpos moves";
    void startEngine(string fileName);
    string lastEngineMove;
    void sendCommand(string command);
    void makeMove(string next_move);
    string readResponse();
    void endProcess();
    string parseEngineResponse();
public:
    ChessEngine(string fileName);
    ~ChessEngine();
    void getUserMove(string move);
    bool readGameover();
    string getEngineMove();
};