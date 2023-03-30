#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#ifndef ALPHACLASS_H
#define ALPHACLASS_H

class alphaclass
{
private:
    sql::Driver* driver;
    std::unique_ptr<sql::Connection> con;
    std::unique_ptr<sql::PreparedStatement> pstmt;
    std::unique_ptr<sql::ResultSet> res;
    std::unique_ptr<sql::Statement> stmt;
public:
    alphaclass();
    alphaclass(int moves);
    void createDatabaseAndTable();
    void insertData();
    void printTable(sql::Connection& con);
    void printer();

};
#endif
