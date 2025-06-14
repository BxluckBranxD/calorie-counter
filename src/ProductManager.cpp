#include "ProductManager.hpp"
#include <iostream>

ProductManager::ProductManager(DataManager& dm) : dataManager(dm) {}

bool ProductManager::addOrEditProduct(const Product& product) {
    return dataManager.upsert(product);
}

std::vector<Product> ProductManager::searchProducts(const std::string& keyword) const {
    return dataManager.search(keyword);
}

bool ProductManager::getProduct(const std::string& name, Product& out) const {
    return dataManager.get(name, out);
}

void ProductManager::printProductDetails(const Product& product) const {
    std::cout << "Название: " << product.name << "\n"
              << "Калории: " << product.calories << " ккал\n"
              << "Белки: " << product.protein << " г\n"
              << "Жиры: " << product.fat << " г\n"
              << "Углеводы: " << product.carbs << " г\n";
}
