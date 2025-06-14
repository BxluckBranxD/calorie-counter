#ifndef DIARYMANAGER_HPP
#define DIARYMANAGER_HPP

#include <string>
#include <vector>
#include "Product.hpp"
#include "DataManager.hpp"
#include "DailyStats.hpp"
struct DiaryEntry {
    std::string date;
    std::string productName;
    double quantity;
};

class DiaryManager {
public:
    explicit DiaryManager(DataManager& dm);
    
    bool addEntry(const std::string& productName, double quantity, const std::string& date);
    std::vector<DiaryEntry> getDailyEntries(const std::string& date) const;
    DailyStats calculateDailyStats(const std::string& date) const;
    void compareWithNorm(const DailyStats& stats) const;

private:
    DataManager& dataManager;
};

#endif // DIARYMANAGER_HPP