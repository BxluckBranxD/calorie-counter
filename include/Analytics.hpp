#pragma once
#include "DiaryManager.hpp"
#include "ProductManager.hpp"

/**
 * @brief Аналитика: сравнение фактических калорий с дневной нормой.
 */
class Analytics {
    double dailyNorm;
public:
    explicit Analytics(double norm);

    /// Сравнивает итог за day с дневной нормой и возвращает разницу.
    double compare(const std::tm& day,
                   const DiaryManager& dm,
                   const ProductManager& pm) const;

    /// Вычисляет среднюю калорийность за неделю.
    double weeklyAverage(const DiaryManager& dm, const ProductManager& pm) const;
};
