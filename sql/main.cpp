#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>


void createDatabaseAndTable() {
    sql::Driver* driver(get_driver_instance()); //p
    std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "sebas", "password"));
    std::unique_ptr<sql::Statement> stmt(con->createStatement());

    // Create database if it doesn't exist
    stmt->execute("CREATE DATABASE IF NOT EXISTS myDB");

    // Switch to the database
    con->setSchema("myDB");

    // Create table if it doesn't exist
    stmt->execute("CREATE TABLE IF NOT EXISTS kunde (kunde_id INT NOT NULL, navn VARCHAR(50), adresse VARCHAR(50), mail VARCHAR(50), tlf INT, PRIMARY KEY (kunde_id))");
}

void insertData(std::string navn, std::string adresse, std::string mail, int tlf) {
    sql::Driver* driver(get_driver_instance());
    std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "sebas", "password"));
    con->setSchema("myDB");

    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT MAX(kunde_id) FROM kunde"));

    int kunde_id = 1;
    if (res->next()) {
        kunde_id = res->getInt(1) + 1;
    }



    std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT IGNORE INTO kunde (kunde_id, navn, adresse, mail, tlf) VALUES (?, ?, ?, ?, ?)"));
    pstmt->setInt(1, kunde_id);
    pstmt->setString(2, navn);
    pstmt->setString(3, adresse);
    pstmt->setString(4, mail);
    pstmt->setInt(5, tlf);
    pstmt->execute();
    pstmt->clearParameters();
}

void printKundeTable(sql::Connection& con) {
    std::unique_ptr<sql::Statement> stmt;
    std::unique_ptr<sql::ResultSet> res;

    stmt = std::unique_ptr<sql::Statement>(con.createStatement());
    res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery("SELECT * FROM kunde"));

    while (res->next()) {
        int kunde_id = res->getInt("kunde_id");
        std::string navn = res->getString("navn");
        std::string adresse = res->getString("adresse");
        std::string mail = res->getString("mail");
        int tlf = res->getInt("tlf");

        std::cout << kunde_id << ", " << navn << ", " << adresse << ", " << mail << ", " << tlf << std::endl;
    }
    res->close();
}




int main() {
    sql::Driver* driver;
    std::unique_ptr<sql::Connection> con;
    std::unique_ptr<sql::PreparedStatement> pstmt;
    std::unique_ptr<sql::ResultSet> res;
    std::unique_ptr<sql::Statement> stmt;

    try {
        driver = get_driver_instance();
        
        con = std::unique_ptr<sql::Connection>(driver->connect("tcp://127.0.0.1:3306", "sebas", "password"));
        
        createDatabaseAndTable();
        con->setSchema("myDB"); 

        std::cout << "\n-1----------------------------------\n" << std::endl;
        
        printKundeTable(*con);
        
        insertData("Tim", "Timvej 10", "tim@mail.tim", 12345678);
        insertData("Lars", "Larsvej 7", "lars@mail.lars", 66612399);

        std::cout << "\n-2----------------------------------\n" << std::endl;

        printKundeTable(*con);
    
        std::cout << "\n-3----------------------------------\n" << std::endl;


        stmt = std::unique_ptr<sql::Statement>(con->createStatement());
        stmt->execute("DELETE FROM kunde WHERE kunde_id > 6");

    
    } catch (sql::SQLException& e) {
        std::cout << "Error connecting to MySQL: " << e.what() << std::endl;
    }

    return 0;
}