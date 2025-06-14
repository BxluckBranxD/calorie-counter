#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../include/DataManager.hpp"

TEST_CASE("Операции DataManager") {
    DataManager dm(":memory:");
    
    SECTION("Добавление и получение продукта") {
        Product p{"Яблоко", 52, 0.3, 0.2, 14};
        REQUIRE(dm.upsert(p));
        
        Product out;
        REQUIRE(dm.get("Яблоко", out));
        REQUIRE(out.name == "Яблоко");
    }
    
    SECTION("Обновление существующего продукта") {
        Product p1{"Банан", 89, 1.1, 0.3, 23};
        REQUIRE(dm.upsert(p1));
        
        Product p2{"Банан", 95, 1.3, 0.4, 25};
        REQUIRE(dm.upsert(p2));
        
        Product out;
        REQUIRE(dm.get("Банан", out));
        REQUIRE(out.calories == Approx(95));
    }
    
    SECTION("Подсчёт общей калорийности") {
        dm.upsert({"Молоко", 42, 3.4, 1.0, 4.8});
        dm.upsert({"Хлеб", 265, 9, 3.2, 49});
        
        REQUIRE(dm.getTotalCalories() == Approx(307));
    }
    
    SECTION("Пустой запрос возвращает все") {
        dm.upsert({"Яйцо", 155, 13, 11, 1.1});
        auto results = dm.search("");
        REQUIRE(results.size() == 1);
    }
    
    SECTION("Поиск по части названия") {
        dm.upsert({"Куриная грудка", 165, 31, 3.6, 0});
        dm.upsert({"Куриные крылышки", 203, 30, 8.1, 0});
        dm.upsert({"Говяжий стейк", 271, 26, 18, 0});
        
        auto results = dm.search("Кури");
        REQUIRE(results.size() == 2);
        REQUIRE(results[0].name.find("Кури") != std::string::npos);
        REQUIRE(results[1].name.find("Кури") != std::string::npos);
    }
}
