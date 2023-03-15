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
    string lastEngineMove;
    string readResponse();
    void sendCommand(string command);
    void endProcess();
    string parseEngineResponse();
    void startEngine(string fileName);
public:
    ChessEngine(string fileName);
    ChessEngine();
    ~ChessEngine();
    string sendUserMove(string move);
    bool readGameover();
    string getFen();
    void makeMove(string next_move);
};