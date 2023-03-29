#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;

int main() {
    sql::Driver* driver;
    std::unique_ptr<sql::Connection> con;
    std::unique_ptr<sql::PreparedStatement> pstmt;
    std::unique_ptr<sql::ResultSet> res;
    std::unique_ptr<sql::Statement> stmt;

    try {
        driver = get_driver_instance();
        
        con = std::unique_ptr<sql::Connection>(driver->connect("tcp://127.0.0.1:3306", "valdemar", "Cola0712"));
        con->setSchema("testdb"); // Set the database schema

        // Prepare the SQL statement
        pstmt = std::unique_ptr<sql::PreparedStatement>(con->prepareStatement("SELECT * FROM kunde"));
        // Execute the query
        res = std::unique_ptr<sql::ResultSet>(pstmt->executeQuery());

        std::cout << "\n---------------------------" << std::endl;

        // Process the results
        while (res->next()) {
            int kunde_id = res->getInt("kunde_id");
            std::string navn = res->getString("navn");
            std::string adresse = res->getString("adresse");
            std::string mail = res->getString("adresse");
            int tlf = res->getInt("tlf");

            std::cout << "ID: " << kunde_id << ", Name: " << navn << ", Adresse: " << adresse <<
            ", Mail: " << mail << ", tlf: " << tlf << std::endl;
        }
        
        // Close the result set, prepared statement, and connection
        //delete res; //dynamisk på heapen, så 'del' mod memory leaks og til genbrug :)
        //delete pstmt;

        std::cout << "\n---------Insert------------" << std::endl;

        //insert new item query
        pstmt = std::unique_ptr<sql::PreparedStatement>(con->prepareStatement("INSERT INTO kunde (kunde_id, navn, adresse, mail, tlf) VALUES (?,?,?,?,?)"));
        pstmt->setInt(1, 5); // set value for first placeholder
        pstmt->setString(2, "Tim"); // set value for second placeholder
        pstmt->setString(3, "Timvej 10"); // set value for third placeholder
        pstmt->setString(4, "tim@mail.tim");
        pstmt->setInt(5, 12345678);  
        pstmt->execute();

        //delete pstmt;

        std::cout << "\n---------Select again------------" << std::endl;


        // Kør select igen
        pstmt = std::unique_ptr<sql::PreparedStatement>(con->prepareStatement("SELECT * FROM kunde"));
        //sql::ResultSet* 
        res = std::unique_ptr<sql::ResultSet>(pstmt->executeQuery());

        while (res->next()) {
            int kunde_id = res->getInt("kunde_id");
            std::string navn = res->getString("navn");
            std::string adresse = res->getString("adresse");
            std::string mail = res->getString("adresse");
            int tlf = res->getInt("tlf");

            std::cout << "ID: " << kunde_id << ", Name: " << navn << ", Adresse: " << adresse <<
            ", Mail: " << mail << ", tlf: " << tlf << std::endl;

        }

        
        //delete res;
        //delete pstmt;

        
        stmt = std::unique_ptr<sql::Statement>(con->createStatement());
        stmt->execute("DELETE FROM kunde WHERE kunde_id = 5");
        //delete stmt;

    
    } catch (sql::SQLException& e) {
        std::cout << "Error connecting to MySQL: " << e.what() << std::endl;
    }

    return 0;
}