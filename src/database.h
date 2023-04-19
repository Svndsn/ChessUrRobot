#include <iostream>
#include <mysql_connection.h>
#pragma once
#include <mysql_driver.h>
#include <mysql_error.h>
#include <cppconn/statement.h>

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
    void win(bool turn);
    void insertMotorData(double ampUsage);
    ~ChessRobotDatabase()
    {
        delete stmt;
        delete con;
    }
};
