#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "alphaclass.h"

alphaclass::alphaclass(int _moves){
    moves = _moves;
}
void alphaclass::createDatabaseAndTable() {
    sql::Driver* driver(get_driver_instance()); //p
    std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "sebas", "password"));
    std::unique_ptr<sql::Statement> stmt(con->createStatement());

    // Create database if it doesn't exist
    stmt->execute("CREATE DATABASE IF NOT EXISTS myDB");

    // Switch to the database
    con->setSchema("myDB");

    // Create table if it doesn't exist
    stmt->execute("CREATE TABLE IF NOT EXISTS game(game_id INT NOT NULL, moves VARCHAR(50), PRIMARY KEY (game_id))");
}

void alphaclass::insertData() {
    sql::Driver* driver(get_driver_instance());
    std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "sebas", "password"));
    con->setSchema("myDB");

    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT MAX(game_id) FROM game"));
    int game_id = 1;
    if (res->next()) {
        game_id = res->getInt(1) + 1;
    }

    std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT IGNORE INTO game (game_id, moves) VALUES (?, ?)"));
    pstmt->setInt(1, game_id);
    pstmt->setInt(2, moves);
    pstmt->execute();
    pstmt->clearParameters();
}

void alphaclass::printTable(sql::Connection& con) {
    std::unique_ptr<sql::Statement> stmt;
    std::unique_ptr<sql::ResultSet> res;

    stmt = std::unique_ptr<sql::Statement>(con.createStatement());
    res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery("SELECT * FROM game"));

    while (res->next()) {
        int game_id = res->getInt("game_id");
        int moves = res->getInt("moves");

        std::cout << game_id << ", " << moves << std::endl;
    }
    res->close();
}


void alphaclass::printer() {
    try {
        driver = get_driver_instance();
        
        con = std::unique_ptr<sql::Connection>(driver->connect("tcp://127.0.0.1:3306", "sebas", "password"));
        
        createDatabaseAndTable();
        con->setSchema("myDB"); 
        
        printTable(*con);
        
        insertData();
        insertData();
        insertData();

        std::cout << "\n------------main-table------------\n" << std::endl;

        printTable(*con);
    
        std::cout << "\n-----------inner-tables-----------\n" << std::endl;


        stmt = std::unique_ptr<sql::Statement>(con->createStatement());
        stmt->execute("DELETE FROM game WHERE game_id > 3");

    
    } catch (sql::SQLException& e) {
        std::cout << "Error connecting to MySQL: " << e.what() << std::endl;
    }
}