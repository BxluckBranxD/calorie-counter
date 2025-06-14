#include "DiaryManager.hpp"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <iostream>
using namespace std;

DiaryManager::DiaryManager(DataManager& dm) : dataManager(dm) {
    // Создаем таблицу для дневника при инициализации
    const char* sql = 
        "CREATE TABLE IF NOT EXISTS diary ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "date TEXT NOT NULL,"
        "product_name TEXT NOT NULL,"
        "quantity REAL NOT NULL,"
        "FOREIGN KEY(product_name) REFERENCES products(name)"
        ");";
    
    char* errMsg = nullptr;
    if (sqlite3_exec(dataManager.db_handle(), sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        string error = "Failed to create diary table: ";
        error += errMsg;
        sqlite3_free(errMsg);
        throw runtime_error(error);
    }
}

bool DiaryManager::addEntry(const string& productName, double quantity, const string& date) {
    // Проверяем существование продукта
    Product p;
    if (!dataManager.get(productName, p)) {
        cerr << "Ошибка: продукт '" << productName << "' не найден в базе." << endl;
        return false;
    }

    if (quantity <= 0) {
        cerr << "Ошибка: количество должно быть положительным." << endl;
        return false;
    }

    // Подготавливаем SQL запрос
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO diary (date, product_name, quantity) VALUES (?, ?, ?)";
    
    if (sqlite3_prepare_v2(dataManager.db_handle(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Ошибка подготовки SQL запроса." << endl;
        return false;
    }

    // Привязываем параметры
    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, productName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, quantity); 

    // Выполняем запрос
    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!result) {
        cerr << "Ошибка при добавлении записи в дневник." << endl;
    }

    return result;
}

vector<DiaryEntry> DiaryManager::getDailyEntries(const string& date) const {
    vector<DiaryEntry> entries;

    // Подготавливаем SQL запрос
    sqlite3_stmt* stmt;
    const char* sql = "SELECT product_name, quantity FROM diary WHERE date = ? ORDER BY id";
    
    if (sqlite3_prepare_v2(dataManager.db_handle(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw runtime_error("Ошибка подготовки SQL запроса для получения записей");
    }

    // Привязываем параметр даты
    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT);

    // Читаем результаты
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        DiaryEntry entry;
        entry.date = date;
        entry.productName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        entry.quantity = sqlite3_column_double(stmt, 1);
        entries.push_back(entry);
    }

    sqlite3_finalize(stmt);
    return entries;
}

DailyStats DiaryManager::calculateDailyStats(const string& date) const {
    DailyStats stats{0, 0, 0, 0};
    vector<DiaryEntry> entries = getDailyEntries(date);

    for (const auto& entry : entries) {
        Product p;
        if (dataManager.get(entry.productName, p)) {
            double ratio = entry.quantity / 100.0;
            stats.calories += p.calories * ratio;
            stats.protein += p.protein * ratio;
            stats.fat += p.fat * ratio;
            stats.carbs += p.carbs * ratio;
        }
    }

    return stats;
}

void DiaryManager::compareWithNorm(const DailyStats& stats) const {
    // Нормы потребления 
    const DailyStats norm{2000, 120, 70, 250}; // ккал, белки(г), жиры(г), углеводы(г)

    cout << fixed << setprecision(1);
    cout << "\n=== Сравнение с дневной нормой ===\n";
    
    // Калории
    cout << "Калории: " << stats.calories << "/" << norm.calories << " ккал ("
         << (stats.calories/norm.calories*100) << "%)\n";
    
    // Белки
    cout << "Белки: " << stats.protein << "/" << norm.protein << " г ("
         << (stats.protein/norm.protein*100) << "%)\n";
    
    // Жиры
    cout << "Жиры: " << stats.fat << "/" << norm.fat << " г ("
         << (stats.fat/norm.fat*100) << "%)\n";
    
    // Углеводы
    cout << "Углеводы: " << stats.carbs << "/" << norm.carbs << " г ("
         << (stats.carbs/norm.carbs*100) << "%)\n";

    // Общая оценка
    double totalPercentage = (stats.calories/norm.calories + 
                            stats.protein/norm.protein + 
                            stats.fat/norm.fat + 
                            stats.carbs/norm.carbs) / 4 * 100;
    
    cout << "\nОбщий баланс: " << totalPercentage << "% от нормы\n";
    
    if (totalPercentage < 80) {
        cout << "Рекомендация: увеличить потребление\n";
    } else if (totalPercentage > 120) {
        cout << "Рекомендация: уменьшить потребление\n";
    } else {
        cout << "Отличный баланс!\n";
    }
}