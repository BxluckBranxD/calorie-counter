#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>

struct Product {
    std::string name;
    double calories;
    double protein;
    double fat;
    double carbs;

    bool operator==(const Product& other) const {
        return name == other.name
            && calories == other.calories
            && protein == other.protein
            && fat == other.fat
            && carbs == other.carbs;
    }
};

#endif // PRODUCT_HPP