#include "SQLInterface.h"

int main() {
    SQLInterface chessDB("tcp://127.0.0.1:3306", "valdemar", "password");

    try {
        chessDB.connect();
        chessDB.printKundeTable();
        chessDB.insertData("Ghita", "Ghitavej 26", "ghita@mail.com", 45454545);
        chessDB.printKundeTable();


    } catch (sql::SQLException& e) {
        std::cout << "Error connecting to MySQL: " << e.what() << std::endl;
    }

    return 0;
}


