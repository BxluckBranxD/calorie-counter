#ifndef DAILYSTATS_HPP
#define DAILYSTATS_HPP

struct DailyStats {
    double calories;
    double protein;
    double fat;
    double carbs;
    
    DailyStats operator+(const DailyStats& other) const {
        return {
            calories + other.calories,
            protein + other.protein,
            fat + other.fat,
            carbs + other.carbs
        };
    }
};

#endif // DAILYSTATS_HPP