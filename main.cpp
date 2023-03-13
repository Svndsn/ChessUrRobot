#include <iostream>
#include <string>
#include "src/ChessEngine.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

using namespace std;

int main()
{
    // Start the engine process
	ChessEngine sf;
    sf.startEngine("stockfish/stockfish-ubuntu-20.04-x86-64");

    // Send the UCI commands to initialize the engine
    sf.readResponse(); // Read init line printed
    sf.sendCommand("uci");
	cout << 1 << endl;
    sf.readResponse();
	cout << 2 << endl;
    sf.sendCommand("isready");
	cout << 3 << endl;
    sf.readResponse();
	cout << 4 << endl;

    // Play the game
    while (sf.readGameover())
    {
        if (sf.readTurn() == "white")
        {
            // Get the user's move
            string move = sf.getUserMove();
            // Make the move on the board
            sf.makeMove(move);
			sf.changeTurn();
        }
        else
        {
            // Ask the engine to suggest a move
            sf.sendCommand("go movetime 1000");
            string response = sf.readResponse();
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
                    sf.makeMove(move);
					sf.changeTurn();
                }
                else
                {
                    // Handle engine error
                }
            }
        }
    }

    // Close the engine process
    sf.endProcess();

    return 0;
}
