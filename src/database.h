#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <cppconn/statement.h>

class ChessRobotDatabase {
private:
    sql::Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    void createDatabase();
    void connect(const std::string& username, const std::string& password);

public:
    ChessRobotDatabase(const std::string& username, const std::string& password);
    void insertGame(const std::string& playerName);

    ~ChessRobotDatabase() {
        delete stmt;
        delete con;
    }
};
