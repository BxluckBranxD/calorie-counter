#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../include/ProductManager.hpp"
#include "../include/DataManager.hpp"

TEST_CASE("Операции ProductManager") {
    DataManager dm(":memory:");
    ProductManager pm(dm);
    
    SECTION("Добавление нового продукта") {
        Product p{"Йогурт", 59, 10, 0.4, 3.6};
        REQUIRE(pm.addOrEditProduct(p));
    }
    
    SECTION("Поиск несуществующего продукта") {
        Product p;
        REQUIRE_FALSE(pm.getProduct("Несуществующий", p));
    }
    
    SECTION("Вывод информации о продукте") {
        Product p{"Сыр", 402, 25, 33, 1.3};
        pm.addOrEditProduct(p);
        
        // Проверяем, что функция выполняется без исключений
        REQUIRE_NOTHROW(pm.printProductDetails(p));
    }
    
    SECTION("Поиск по части названия") {
        pm.addOrEditProduct({"Апельсиновый сок", 45, 0.7, 0.2, 10});
        auto results = pm.searchProducts("Апельсин");
        REQUIRE(results.size() == 1);
    }
    
    SECTION("Редактирование существующего продукта") {
        Product original{"Помидор", 18, 0.9, 0.2, 3.9};
        REQUIRE(pm.addOrEditProduct(original));
        
        Product modified{"Помидор", 20, 1.0, 0.3, 4.0};
        REQUIRE(pm.addOrEditProduct(modified));
        
        Product retrieved;
        REQUIRE(pm.getProduct("Помидор", retrieved));
        REQUIRE(retrieved.calories == Approx(20));
        REQUIRE(retrieved.protein  == Approx(1.0));
    }
}
