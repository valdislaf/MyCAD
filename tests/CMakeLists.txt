cmake_minimum_required(VERSION 3.19)
project(MyCADTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(GTest REQUIRED)

include_directories(../MyCAD)

add_executable(runTests
    test_shapemanager.cpp
    ../MyCAD/ShapeManager.cpp
)

target_link_libraries(runTests PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets GTest::gtest GTest::gtest_main)

add_test(NAME runTests COMMAND runTests)
