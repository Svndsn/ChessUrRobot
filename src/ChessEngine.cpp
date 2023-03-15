#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include "ChessEngine.h"

using namespace std;

#define PIPE_READ 0
#define PIPE_WRITE 1

void ChessEngine::startEngine(string fileName)
{
    int infd[2];
    int outfd[2];
    int nChild;
    int nResult;

    // Create input and output pipes
    if (pipe(infd) < 0)
    {
        perror("allocating pipe for child input redirect");
        exit(1);
    }
    if (pipe(outfd) < 0)
    {
        close(infd[PIPE_READ]);
        close(infd[PIPE_WRITE]);
        perror("allocating pipe for child output redirect");
        exit(1);
    }

    // Fork a child process
    nChild = fork();
    if (nChild == -1)
    {
        // Error creating child process
        perror("fork");
        exit(1);
    }
    else if (nChild == 0)
    {
        // Child process
        close(infd[PIPE_WRITE]);
        close(outfd[PIPE_READ]);

        // Redirect standard input, output, and error to pipes
        if (dup2(infd[PIPE_READ], STDIN_FILENO) == -1)
        {
            perror("dup2 stdin");
            exit(1);
        }
        if (dup2(outfd[PIPE_WRITE], STDOUT_FILENO) == -1)
        {
            perror("dup2 stdout");
            exit(1);
        }
        if (dup2(outfd[PIPE_WRITE], STDERR_FILENO) == -1)
        {
            perror("dup2 stderr");
            exit(1);
        }

        // Close unused pipe ends
        close(infd[PIPE_READ]);
        close(outfd[PIPE_WRITE]);

        // Execute the chess engine program
        char *argv[] = {const_cast<char *>(fileName.c_str()), NULL};
        nResult = execve(fileName.c_str(), argv, NULL);
        if (nResult == -1)
        {
            perror("execve");
            exit(1);
        }
    }
    else
    {
        // Parent process
        close(infd[PIPE_READ]);
        close(outfd[PIPE_WRITE]);

        // Save process information
        process *proc = new process;
        proc->pid = nChild;
        proc->infd = infd[PIPE_WRITE];
        proc->outfd = outfd[PIPE_READ];
        engineProcess = proc;
    }
}

void ChessEngine::sendCommand(string command)
{
    cout << "sending command: " << command << endl;
    cout.flush();
    // fprintf(engineProcess->infd, "%s\n", command.c_str());
    write(engineProcess->infd, command.c_str(), command.length());
    write(engineProcess->infd, "\n", 1);
}

string ChessEngine::readResponse()
{
    char buffer[4096] = {0};
    int resp_len = read(engineProcess->outfd, buffer, sizeof(buffer));
    string response(buffer, resp_len);
    cout << "received response: " << response << endl;
    return response;
}

void ChessEngine::makeMove(string next_move)
{
    board += " " + next_move;
    sendCommand(board);
}

string ChessEngine::sendUserMove(string move)
{
    makeMove(move);
    sendCommand("go movetime 1000");
    return parseEngineResponse();
    
}

void ChessEngine::endProcess()
{
    close(engineProcess->infd);
    close(engineProcess->outfd);
    delete engineProcess;
}

bool ChessEngine::readGameover()
{
    return gameover;
}

ChessEngine::ChessEngine(string fileName)
{
    startEngine(fileName);
    readResponse();
    sendCommand("isready");
    readResponse();
}

string ChessEngine::parseEngineResponse()
{
    string move;
    while (1)
    {
        string response = readResponse();
        size_t bestmoveAt = response.find("bestmove");
        if (bestmoveAt != string::npos)
        {
            string bestmoveString = response.substr(bestmoveAt - 1, bestmoveAt + 7);
            int end = bestmoveString.find("ponder") - 11;
            move = bestmoveString.substr(10, end);
            break;
        }
    }
    return move;
}


ChessEngine::~ChessEngine()
{
    endProcess();
}

ChessEngine::ChessEngine() {}

string ChessEngine::getFen()
{
    sendCommand("d");
    string response = readResponse();
    int start = response.find("Fen: ")+5;
    int stop = response.find(" ",start) - start;
    return response.substr(start,stop);
}

bool ChessEngine::doesMoveKill(const string &fen, const string &move_str)
{
    // Split the FEN string into its components
    std::vector<std::string> fen_components;
    std::string component;
    for (char c : fen)
    {
        if (c == ' ')
        {
            fen_components.push_back(component);
            component = "";
        }
        else
        {
            component += c;
        }
    }
    fen_components.push_back(component);

    // Get the current board position
    std::string position = fen_components[0];

    // Apply the move to the board position
    int from_file = move_str[0] - 'a';
    int from_rank = 8 - (move_str[1] - '0');
    int to_file = move_str[2] - 'a';
    int to_rank = 8 - (move_str[3] - '0');
    int from_square = from_rank * 8 + from_file;
    int to_square = to_rank * 8 + to_file;
    char piece = position[from_square];
    position[from_square] = ' ';
    position[to_square] = piece;

    // Check if a piece was captured
    bool piece_captured = false;
    for (size_t i = 0; i < position.size(); ++i)
    {
        if (position[i] != fen_components[0][i])
        {
            // Check if a piece is missing in the new position
            if (position[i] == ' ')
            {
                piece_captured = true;
                break;
            }
        }
    }
    return piece_captured;
}