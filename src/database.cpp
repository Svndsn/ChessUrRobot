#include "database.h"

ChessRobotDatabase::ChessRobotDatabase(const std::string& username, const std::string& password){
    {
        try {
            connect(username,password);
            createDatabase();
        }
        catch (sql::SQLException& e) {
            std::cerr << "SQL error: " << e.what() << std::endl;
            std::cerr << "MySQL error code: " << e.getErrorCode() << std::endl;
        }
    }
}

void ChessRobotDatabase::createDatabase()
{
    // Create the database if it doesn't exist
            stmt->execute("CREATE DATABASE IF NOT EXISTS chessRobot");
            stmt->execute("USE chessRobot");

            // Create the game table if it doesn't exist
            stmt->execute("CREATE TABLE IF NOT EXISTS game ("
                          "game_id INT AUTO_INCREMENT PRIMARY KEY,"
                          "playerName VARCHAR(50),"
                          "date DATE"
                          ")");

            // Create the moves table if it doesn't exist
            stmt->execute("CREATE TABLE IF NOT EXISTS moves ("
                          "move_id INT AUTO_INCREMENT PRIMARY KEY,"
                          "game_id INT,"
                          "move VARCHAR(50),"
                          "turn BOOL,"
                          "isKill BOOL,"
                          "FOREIGN KEY (game_id) REFERENCES game(game_id)"
                          ")");

            // Create the motordata table if it doesn't exist
            stmt->execute("CREATE TABLE IF NOT EXISTS motordata ("
                          "motordata_id INT AUTO_INCREMENT PRIMARY KEY,"
                          "ampUsage DOUBLE,"
                          "move_id INT,"
                          "FOREIGN KEY (move_id) REFERENCES moves(move_id)"
                          ")");
}

void ChessRobotDatabase::connect(const std::string& username, const std::string& password) {
    // Create a MySQL driver object and connect to the database
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", username, password);
    stmt = con->createStatement();
}

void ChessRobotDatabase::insertGame(const std::string& playerName) {
    try {
        // Get the current date in YYYY-MM-DD format
        time_t now = time(nullptr);
        char dateBuffer[11];
        strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", localtime(&now));
        std::string date(dateBuffer);

        // Construct the SQL statement and execute it
        std::string sql = "INSERT INTO game (playerName, date) VALUES ('" + playerName + "', '" + date + "')";
        stmt->execute(sql);
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "MySQL error code: " << e.getErrorCode() << std::endl;
    }
}
