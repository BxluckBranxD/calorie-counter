#include "DataManager.hpp"
#include <stdexcept>
#include <iostream>
#include "../include/DatabaseUtils.hpp"
using namespace std;


DataManager::DataManager(const string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        throw runtime_error("Cannot open database");
    }
    createTables();      // Создаем таблицу продуктов
    createDiaryTable(db); // Создаем таблицу дневника
}

DataManager::~DataManager() {
    sqlite3_close(db);
}

void DataManager::createTables() {
    const char* sql = 
        "CREATE TABLE IF NOT EXISTS products ("
        "name TEXT PRIMARY KEY,"
        "calories REAL,"
        "protein REAL," 
        "fat REAL,"
        "carbs REAL"
        ");";
    
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        string error = "Failed to create tables: ";
        error += errMsg;
        sqlite3_free(errMsg);
        throw runtime_error(error);
    }
}

bool DataManager::upsert(const Product& product) {
    const char* sql = 
        "INSERT OR REPLACE INTO products VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare SQL statement" << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, product.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, product.calories);
    sqlite3_bind_double(stmt, 3, product.protein);
    sqlite3_bind_double(stmt, 4, product.fat);
    sqlite3_bind_double(stmt, 5, product.carbs);

    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    
    if (!result) {
        cerr << "Failed to upsert product: " << sqlite3_errmsg(db) << endl;
    }
    
    return result;
}

bool DataManager::get(const string& name, Product& out) const {
    const char* sql = "SELECT * FROM products WHERE name = ?";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare SQL statement" << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    
    bool found = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        out.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        out.calories = sqlite3_column_double(stmt, 1);
        out.protein = sqlite3_column_double(stmt, 2);
        out.fat = sqlite3_column_double(stmt, 3);
        out.carbs = sqlite3_column_double(stmt, 4);
        found = true;
    }
    
    sqlite3_finalize(stmt);
    return found;
}

vector<Product> DataManager::search(const string& query) const {
    vector<Product> results;
    const char* sql = "SELECT * FROM products WHERE name LIKE ?";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare SQL statement" << endl;
        return results;
    }

    string likeQuery = "%" + query + "%";
    sqlite3_bind_text(stmt, 1, likeQuery.c_str(), -1, SQLITE_TRANSIENT);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Product p;
        p.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        p.calories = sqlite3_column_double(stmt, 1);
        p.protein = sqlite3_column_double(stmt, 2);
        p.fat = sqlite3_column_double(stmt, 3);
        p.carbs = sqlite3_column_double(stmt, 4);
        results.push_back(p);
    }
    
    sqlite3_finalize(stmt);
    return results;
}

double DataManager::getTotalCalories() const {
    double total = 0.0;
    const char* sql = "SELECT SUM(calories) FROM products";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            total = sqlite3_column_double(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    
    return total;
}