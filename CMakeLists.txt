cmake_minimum_required(VERSION 3.10)
project(CalorieCounter)

set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)

# Основная библиотека
add_library(calorie_core
    src/DataManager.cpp
    src/ProductManager.cpp
    src/DataManager.cpp
    src/ProductManager.cpp
    src/DiaryManager.cpp
    src/DatabaseUtils.cpp 
)

target_include_directories(calorie_core PUBLIC include)
target_link_libraries(calorie_core PRIVATE SQLite::SQLite3)

# Основное приложение
add_executable(calorie_counter src/main.cpp)
target_link_libraries(calorie_counter PRIVATE calorie_core)

# Тесты

enable_testing()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_datamanager.cpp")
    add_executable(test_datamanager tests/test_datamanager.cpp)
    target_link_libraries(test_datamanager PRIVATE calorie_core)
    add_test(NAME DataManagerTests COMMAND test_datamanager)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_productmanager.cpp")
    add_executable(test_productmanager tests/test_productmanager.cpp)
    target_link_libraries(test_productmanager PRIVATE calorie_core)
    add_test(NAME ProductManagerTests COMMAND test_productmanager)
endif()