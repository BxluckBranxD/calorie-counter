#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <sqlite3.h>
#include <string>
#include <vector>
#include "Product.hpp"

class DataManager {
public:
    explicit DataManager(const std::string& dbPath = ":memory:");
    ~DataManager();

    bool upsert(const Product& product);
    bool get(const std::string& name, Product& out) const;
    std::vector<Product> search(const std::string& query) const;
    double getTotalCalories() const;
    sqlite3* db_handle() const { return db; }

private:
    sqlite3* db;
    void createTables();
};


#endif // DATAMANAGER_HPP