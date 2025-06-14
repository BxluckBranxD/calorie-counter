#ifndef PRODUCTMANAGER_HPP
#define PRODUCTMANAGER_HPP

#include <vector>
#include <string>
#include "Product.hpp"
#include "DataManager.hpp"

class ProductManager {
public:
    explicit ProductManager(DataManager& dm);  // Измененный конструктор
    bool addOrEditProduct(const Product& product);
    std::vector<Product> searchProducts(const std::string& keyword) const;
    bool getProduct(const std::string& name, Product& out) const;
    void printProductDetails(const Product& product) const;

private:
    DataManager& dataManager;  // Теперь ссылка
};

#endif // PRODUCTMANAGER_HPP