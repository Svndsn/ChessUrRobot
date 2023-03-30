#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include "SQLInterface.h"
#include <string>

SQLInterface::SQLInterface(std::string host, std::string username, std::string password) : host_(host), username_(username), password_(password) {}

void SQLInterface::connect()
{
    driver_ = get_driver_instance();
    con_ = std::unique_ptr<sql::Connection>(driver_->connect(host_, username_, password_));
    //con_->setSchema(schema_);
    std::unique_ptr<sql::Statement> stmt(con_->createStatement());

    std::string dbName = "myDBtest";
    std::string createStmt = "CREATE DATABASE IF NOT EXISTS ";
    createStmt.append(dbName);


    // Create database if it doesn't exist
    stmt->execute(createStmt);   //

    // Switch to the database
    con_->setSchema(dbName);

    // Create table if it doesn't exist
    stmt->execute("CREATE TABLE IF NOT EXISTS kunde (kunde_id INT NOT NULL, navn VARCHAR(50), adresse VARCHAR(50), mail VARCHAR(50), tlf INT, PRIMARY KEY (kunde_id))");
}


void SQLInterface::insertData(std::string navn, std::string adresse, std::string mail, int tlf)
{
     std::unique_ptr<sql::Statement> stmt(con_->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT MAX(kunde_id) FROM kunde"));

    int kunde_id = 1;
    if (res->next()) {
        kunde_id = res->getInt(1) + 1;
    }

    std::unique_ptr<sql::PreparedStatement> pstmt(con_->prepareStatement("INSERT INTO kunde (kunde_id, navn, adresse, mail, tlf) VALUES (?, ?, ?, ?, ?)"));
    pstmt->setInt(1, kunde_id);
    pstmt->setString(2, navn);
    pstmt->setString(3, adresse);
    pstmt->setString(4, mail);
    pstmt->setInt(5, tlf);
    pstmt->execute();
    pstmt->clearParameters();
}

void SQLInterface::printKundeTable()
{
    std::unique_ptr<sql::Statement> stmt(con_->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM kunde"));
    while (res->next())
    {
        int kunde_id = res->getInt("kunde_id");
        std::string navn = res->getString("navn");
        std::string adresse = res->getString("adresse");
        std::string mail = res->getString("mail");
        int tlf = res->getInt("tlf");
        std::cout << kunde_id << ", " << navn << ", " << adresse << ", " << mail << ", " << tlf << std::endl;
    }
    res->close();
}

void SQLInterface::deleteKundeTable()
{
    std::unique_ptr<sql::Statement> stmt(con_->createStatement());
    stmt->execute("DELETE FROM kunde WHERE kunde_id > 4");
}