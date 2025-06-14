#include "Analytics.hpp"

Analytics::Analytics(double norm)
    : dailyNorm(norm)
{}

double Analytics::compare(const std::tm& day,
                          const DiaryManager& dm,
                          const ProductManager& pm) const
{
    Product sum = dm.summarizeDay(day, pm);
    return sum.calories - dailyNorm;
}

double Analytics::weeklyAverage(const DiaryManager& dm,
                                const ProductManager& pm) const
{
    return dm.averageCaloriesLastWeek(pm);
}
