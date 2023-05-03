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
    //cout << "sending command: " << command << endl;
    //cout.flush();
    // fprintf(engineProcess->infd, "%s\n", command.c_str());
    write(engineProcess->infd, command.c_str(), command.length());
    write(engineProcess->infd, "\n", 1);
}

string ChessEngine::readResponse()
{
    char buffer[8192] = {0};
    int resp_len = read(engineProcess->outfd, buffer, sizeof(buffer));
    string response(buffer, resp_len);
    //cout << "received response: " << response << endl;
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
    sendCommand("go movetime 100");
    bool gameover;
    while (true)
    {
        
        string response = readResponse();
        size_t buff = response.find("bestmove");
        if (buff != string::npos)
        {
            string buffGameover = response.substr(buff - 1, buff + 7);
            if (buffGameover.substr(10,6)=="(none)")
            {
                gameover = true;
                break;
            } else {
                gameover = false;
                break;
            }
            
        }
        
    }
    
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
    while (true)
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
    int stop = response.find(" ",start)+2 - start;
    return response.substr(start,stop);
}
      