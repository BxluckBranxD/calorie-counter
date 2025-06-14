#include "../include/DatabaseUtils.hpp"

void createDiaryTable(sqlite3* db) {
    const char* sql = 
        "CREATE TABLE IF NOT EXISTS diary ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "date TEXT NOT NULL,"
        "product_name TEXT NOT NULL,"
        "quantity REAL NOT NULL,"
        "FOREIGN KEY(product_name) REFERENCES products(name)"
        ");";
    
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = "Failed to create diary table: ";
        error += errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}