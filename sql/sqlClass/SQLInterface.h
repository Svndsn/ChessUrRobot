#pragma once
#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>

class SQLInterface
{
private:
    sql::Driver* driver_;
    std::unique_ptr<sql::Connection> con_;
    std::string host_;
    std::string username_;
    std::string password_;
    std::string schema_;
public:
    SQLInterface(std::string host, std::string username, std::string password, std::string schema);
    void connect();
    void createDatabase();
    void insertData(std::string navn, std::string adresse, std::string mail, int tlf);
    void printKundeTable();
    void deleteKundeTable();
};

