#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <errno.h>
#include "ChessEngine.h"
using namespace std;

#define PIPE_READ 0
#define PIPE_WRITE 1

void ChessEngine::startEngine(string fileName) {
    int infd[2];
	int outfd[2];
	int nChild;
	int nResult;
	if (pipe(infd) < 0) {
		perror("allocating pipe for child input redirect");
		exit(1);
	}
	if (pipe(outfd) < 0) {
		close(infd[PIPE_READ]);
		close(infd[PIPE_WRITE]);
		perror("allocating pipe for child output redirect");
		exit(1);
	}

	nChild = fork();
	if (0 == nChild) {
		// child continues here

		// redirect stdin
		if (dup2(infd[PIPE_READ], STDIN_FILENO) == -1) {
			exit(errno);
		}

		// redirect stdout
		if (dup2(outfd[PIPE_WRITE], STDOUT_FILENO) == -1) {
			exit(errno);
		}

		// redirect stderr
		if (dup2(outfd[PIPE_WRITE], STDERR_FILENO) == -1) {
			exit(errno);
		}

		// all these are for use by parent only
		close(infd[PIPE_READ]);
		close(infd[PIPE_WRITE]);
		close(outfd[PIPE_READ]);
		close(outfd[PIPE_WRITE]); 

		// run child process image
		// replace this with any exec* function find easier to use ("man exec")
		nResult = execve(fileName.c_str(), NULL, NULL);

		// if we get here at all, an error occurred, but we are in the child
		// process, so just exit
		exit(nResult);
	} else if (nChild > 0) {
		// parent continues here

		// close unused file descriptors, these are for child only
		close(infd[PIPE_READ]);
		close(outfd[PIPE_WRITE]); 


	} else {
		// failed to create child
		close(infd[PIPE_READ]);
		close(infd[PIPE_WRITE]);
		close(outfd[PIPE_READ]);
		close(outfd[PIPE_WRITE]);
	}
	//return NULL;
	process* proc = new process;
	proc->pid = nChild;
	proc->infd = infd[PIPE_WRITE];
	proc->outfd = outfd[PIPE_READ];
	engineProcess = proc;
}

void ChessEngine::sendCommand(string command) {
    cout << "sending command: " << command << endl;
	cout.flush();
	//fprintf(engineProcess->infd, "%s\n", command.c_str());
	write(engineProcess->infd, command.c_str(), command.length());
	write(engineProcess->infd, "\n", 1);
}

string ChessEngine::readResponse()
{
	char buffer[4096] = {0};
	int resp_len = read(engineProcess->outfd,buffer, sizeof(buffer));
	string response(buffer, resp_len);
	cout << "received response: " << response << endl;
    return response;
}

void ChessEngine::makeMove(string next_move)
{
    cout << "Making the move " << next_move << endl;
	board += " " + next_move;
	sendCommand(board);
}

string ChessEngine::getUserMove()
{
    string move;
    cout << "Enter your move: ";
    getline(cin, move);
    return move;
}

void ChessEngine::changeTurn() {
    if (turn=="white")
    {
        turn = "black";
    } else {
        turn = "white";
    }
    
}

void ChessEngine::endProcess() {
	close(engineProcess->infd);
	close(engineProcess->outfd);
	delete engineProcess;
}

bool ChessEngine::readGameover() {
    return gameover;
}

string ChessEngine::readTurn() {
    return turn;
}