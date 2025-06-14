#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>
#include "ProductManager.hpp"
#include "DiaryManager.hpp"

using namespace std;

string getCurrentDate() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
    return string(buffer);
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void showMainMenu() {
    cout << "\n=== Главное меню ==="
         << "\n1. Управление продуктами"
         << "\n2. Дневник питания"
         << "\n3. Аналитика"
         << "\n4. Выход"
         << "\nВыберите действие: ";
}

void showProductsMenu() {
    cout << "\n=== Управление продуктами ==="
         << "\n1. Добавить/изменить продукт"
         << "\n2. Найти продукт"
         << "\n3. Показать информацию о продукте"
         << "\n4. Назад"
         << "\nВыберите действие: ";
}

void showDiaryMenu() {
    cout << "\n=== Дневник питания ==="
         << "\n1. Добавить прием пищи"
         << "\n2. Просмотреть дневные записи"
         << "\n3. Назад"
         << "\nВыберите действие: ";
}

void showAnalyticsMenu() {
    cout << "\n=== Аналитика ==="
         << "\n1. Статистика за день"
         << "\n2. Сравнение с нормой"
         << "\n3. Назад"
         << "\nВыберите действие: ";
}

void manageProducts(ProductManager& pm) {
    int choice;
    while (true) {
        showProductsMenu();
        cin >> choice;
        clearInputBuffer();
        
        switch (choice) {
            case 1: {
                Product p;
                cout << "\nНазвание продукта: ";
                getline(cin, p.name);
                cout << "Калории на 100г: ";
                cin >> p.calories;
                cout << "Белки на 100г: ";
                cin >> p.protein;
                cout << "Жиры на 100г: ";
                cin >> p.fat;
                cout << "Углеводы на 100г: ";
                cin >> p.carbs;
                clearInputBuffer();
                
                if (pm.addOrEditProduct(p)) {
                    cout << "Продукт сохранен!\n";
                } else {
                    cout << "Ошибка сохранения!\n";
                }
                break;
            }
            case 2: {
                string query;
                cout << "\nВведите название продукта для поиска: ";
                getline(cin, query);
                
                auto results = pm.searchProducts(query);
                if (results.empty()) {
                    cout << "Продукты не найдены.\n";
                } else {
                    cout << "\nНайденные продукты:\n";
                    for (const auto& p : results) {
                        cout << "- " << p.name << " (" << p.calories << " ккал)\n";
                    }
                }
                break;
            }
            case 3: {
                string name;
                cout << "\nВведите название продукта: ";
                getline(cin, name);
                
                Product p;
                if (pm.getProduct(name, p)) {
                    cout << "\n=== Информация о продукте ===\n";
                    pm.printProductDetails(p);
                } else {
                    cout << "Продукт не найден.\n";
                }
                break;
            }
            case 4:
                return;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}

void manageDiary(DiaryManager& dm, const string& date) {
    int choice;
    while (true) {
        showDiaryMenu();
        cin >> choice;
        clearInputBuffer();
        
        switch (choice) {
            case 1: {
                string productName;
                double quantity;
                
                cout << "\nВведите название продукта: ";
                getline(cin, productName);
                cout << "Введите количество (в граммах): ";
                cin >> quantity;
                clearInputBuffer();
                
                if (dm.addEntry(productName, quantity, date)) {
                    cout << "Прием пищи добавлен в дневник.\n";
                } else {
                    cout << "Ошибка: продукт не найден или произошла ошибка сохранения.\n";
                }
                break;
            }
            case 2: {
                auto entries = dm.getDailyEntries(date);
                if (entries.empty()) {
                    cout << "\nНа " << date << " записи отсутствуют.\n";
                } else {
                    cout << "\n=== Записи за " << date << " ===\n";
                    for (const auto& entry : entries) {
                        cout << "- " << entry.productName << ": " << entry.quantity << "г\n";
                    }
                }
                break;
            }
            case 3:
                return;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}

void manageAnalytics(DiaryManager& dm, const string& date) {
    int choice;
    while (true) {
        showAnalyticsMenu();
        cin >> choice;
        clearInputBuffer();
        
        switch (choice) {
            case 1: {
                auto stats = dm.calculateDailyStats(date);
                cout << fixed << setprecision(1);
                cout << "\n=== Статистика за " << date << " ===\n"
                     << "Калории: " << stats.calories << " ккал\n"
                     << "Белки: " << stats.protein << " г\n"
                     << "Жиры: " << stats.fat << " г\n"
                     << "Углеводы: " << stats.carbs << " г\n";
                break;
            }
            case 2: {
                auto stats = dm.calculateDailyStats(date);
                dm.compareWithNorm(stats);
                break;
            }
            case 3:
                return;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}

int main() {
    DataManager dm("calorie_db.sqlite");
    ProductManager pm(dm);
    DiaryManager diary(dm);
    
    string currentDate = getCurrentDate();
    int choice;
    
    cout << "=== Счетчик калорий ===\n";
    cout << "Сегодняшняя дата: " << currentDate << "\n";
    
    while (true) {
        showMainMenu();
        cin >> choice;
        clearInputBuffer();
        
        switch (choice) {
            case 1:
                manageProducts(pm);
                break;
            case 2:
                manageDiary(diary, currentDate);
                break;
            case 3:
                manageAnalytics(diary, currentDate);
                break;
            case 4:
                cout << "Выход из программы...\n";
                return 0;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}