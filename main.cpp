#include <iostream>
#include <string>
#include "src/ChessEngine.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

using namespace std;

int main()
{
    // Start the engine process
	ChessEngine sf("stockfish/stockfish-ubuntu-20.04-x86-64");

    return 0;
}
