#include "database.h"

int main() {
    ChessRobotDatabase chessDB("root", "password");
    chessDB.newGame("Simon");
    chessDB.insertMove("e2e4",1,0);
    chessDB.insertMove("e7e5",0,0);

    return 0;
}


