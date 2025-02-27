# Минимальная версия CMake
cmake_minimum_required(VERSION 3.16)

# Название проекта
project(MyCAD LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Установка флагов компиляции
add_definitions(-DQT_DISABLE_DEPRECATED_UP_TO=0x060000)

# Включение автоматической обработки .ui, .qrc и moc файлов
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Поиск Qt6 компонентов
set(Qt6_DIR "C:/Qt/6.8.0/msvc2022_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)


# Установка путей для MinGW
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}             # Текущая директория для исходников
)
message(STATUS "Using Qt6 include dir: ${Qt6Core_INCLUDE_DIRS}")
message(STATUS "Using Qt6 libraries: ${Qt6Core_LIBRARIES}")

# Перечень заголовочных файлов
set(HEADERS
    DrawingManager.h
    DrawingWidget.h
    EventHandling.h
    Grid.h
    HandleManager.h
    MenuInit.h
    MenuMain.h
    MyCAD.h
    Shape.h
    ShapeCircle.h
    ShapeLine.h
    ShapeManager.h
)

# Перечень исходных файлов
set(SOURCES
    DrawingManager.cpp
    DrawingWidget.cpp
    EventHandling.cpp
    Grid.cpp
    HandleManager.cpp
    main.cpp
    MenuInit.cpp
    MenuMain.cpp
    MyCAD.cpp
    Shape.cpp
    ShapeCircle.cpp
    ShapeLine.cpp
    ShapeManager.cpp
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

# Подключение Qt библиотек
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
