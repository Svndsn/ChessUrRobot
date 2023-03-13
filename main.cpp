#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <errno.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

using namespace std;

/* 	
	This application needs to be in the folder of the engine
	the startProcess(%%) function is hard to get working 
	if the targeted process is not in the same folder
*/

string board = "position startpos moves";

struct process {
	int pid;
	int infd;
	int outfd;
};

process* startProcess(string fileName) {
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
	return proc;
}

void endProcess(process* proc) {
	close(proc->infd);
	close(proc->outfd);
	delete proc;
}

// Define a function to send a UCI command to the engine
void sendCommand(process *engineProcess, string command)
{
	cout << "sending command: " << command << endl;
	cout.flush();
	//fprintf(engineProcess->infd, "%s\n", command.c_str());
	write(engineProcess->infd, command.c_str(), command.length());
	write(engineProcess->infd, "\n", 1);
}

// Define a function to read a response from the engine
string readResponse(process *engineProcess)
{
	char buffer[4096] = {0};
	int resp_len = read(engineProcess->outfd,buffer, sizeof(buffer));
	string response(buffer, resp_len);
	cout << "received response: " << response << endl;
    return response;
}

// Define a function to print the current board
void printBoard(process *engineProcess)
{
    cout << "Printing the current board...\n";
    sendCommand(engineProcess, "d");
}

void makeMove(process *engineProcess, string next_move)
{
    cout << "Making the move " << next_move << endl;
	board += " " + next_move;
	sendCommand(engineProcess, board);
}

string getUserMove()
{
    string move;
    cout << "Enter your move: ";
    getline(cin, move);
    return move;
}

int main()
{
    // Start the engine process
    process* engineProcess = startProcess("./stockfish-ubuntu-20.04-x86-64");
    if (!engineProcess)
    {
        cerr << "failed to start engine process" << endl;
        return 1;
    }


    // Send the UCI commands to initialize the engine
    readResponse(engineProcess); // Read init line printed
    sendCommand(engineProcess, "uci");
	cout << 1 << endl;
    readResponse(engineProcess);
	cout << 2 << endl;
    sendCommand(engineProcess, "isready");
	cout << 3 << endl;
    readResponse(engineProcess);
	cout << 4 << endl;

    // Set up the game state
    string turn = "white";
    bool gameover = false;

    // Play the game
    while (!gameover)
    {
        printBoard(engineProcess);
        if (turn == "white")
        {
            // Get the user's move
            string move = getUserMove();
            // Make the move on the board
            makeMove(engineProcess, move);
            turn = "black";
        }
        else
        {
            // Ask the engine to suggest a move
            sendCommand(engineProcess, "go movetime 1000");
            string response = readResponse(engineProcess);
            if (response.substr(0, 8) == "bestmove")
            {
                size_t pos = response.find("bestmove ");
                if (pos != string::npos && response.length() > pos + 9)
                {
                    string move = response.substr(pos + 9, 4);
                    // Extract the actual move from the bestmove substring
                    for (int i = 4; i < response.length() - (pos + 9); i++)
                    {
                        char c = response[pos + 9 + i];
                        if (c == ' ' || c == '\r' || c == '\n')
                        {
                            break;
                        }
                        move += c;
                    }
                    // Make the engine's suggested move on the board
                    makeMove(engineProcess, move);
                    turn = "white";
                }
                else
                {
                    // Handle engine error
                }
            }
        }
    }

    // Close the engine process
    endProcess(engineProcess);

    return 0;
}
