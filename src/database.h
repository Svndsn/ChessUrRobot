#include <iostream>
#include </usr/include/mysql-cppconn-8/jdbc/mysql_connection.h>
#pragma once
#include </usr/include/mysql-cppconn-8/jdbc/mysql_driver.h>
#include </usr/include/mysql-cppconn-8/jdbc/mysql_error.h>
#include </usr/include/mysql-cppconn-8/jdbc/cppconn/statement.h>

class ChessRobotDatabase
{
private:
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    void createDatabase();
    void connect(const std::string &username, const std::string &password);
    int game_id;
    void getGameIdFromDatabase();

public:
    ChessRobotDatabase(const std::string &username, const std::string &password);
    void newGame(const std::string &playerName);
    void insertMove(const std::string &move, bool turn, bool isKill);
    void win(const std::string &whoWon);
    void insertMotorData(double ampUsage);
    void printGame_id();
    ~ChessRobotDatabase()
    {
        delete stmt;
        delete con;
    }
};
