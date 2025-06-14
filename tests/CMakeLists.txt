cmake_minimum_required(VERSION 3.10)
project(CalorieCounter)

set(CMAKE_CXX_STANDARD 17)
find_package(SQLite3 REQUIRED)

# Основная библиотека
add_library(core_lib
    src/DataManager.cpp
    src/ProductManager.cpp
)

target_include_directories(core_lib PUBLIC include)
target_link_libraries(core_lib PRIVATE SQLite::SQLite3)

# Исполняемый файл
add_executable(calorie_counter src/main.cpp)
target_link_libraries(calorie_counter PRIVATE core_lib)

# Добавьте это после основного кода
enable_testing()

# Тесты для ProductManager
add_executable(test_product_manager test_product_manager.cpp)
target_link_libraries(test_product_manager PRIVATE core_lib Catch2::Catch2)
add_test(NAME product_manager COMMAND test_product_manager)

# Тесты для DataManager
add_executable(test_datamanager test_datamanager.cpp)
target_link_libraries(test_datamanager PRIVATE core_lib Catch2::Catch2 SQLite::SQLite3)
add_test(NAME datamanager COMMAND test_datamanager)
