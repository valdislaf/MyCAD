# Минимальная версия CMake
cmake_minimum_required(VERSION 3.19)  # Рекомендуется версия CMake 3.19 или выше

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
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

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
