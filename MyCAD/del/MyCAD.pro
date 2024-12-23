TEMPLATE = app
TARGET = MyCAD
QT += core gui widgets
CONFIG += c++17
INCLUDEPATH += .
DEFINES += QT_DISABLE_DEPRECATED_UP_TO=0x060000
QMAKE_LIBDIR += /mnt/c/Qt/mingw_64/lib
QMAKE_INCDIR += /mnt/c/Qt/mingw_64/include

HEADERS += \
    DrawingManager.h \
    DrawingWidget.h \
    EventHandling.h \
    Grid.h \
    HandleManager.h \
    MenuInit.h \
    MenuMain.h \
    MyCAD.h \
    Shape.h \
    ShapeCircle.h \
    ShapeLine.h \
    ShapeManager.h

SOURCES += \
    DrawingManager.cpp \
    DrawingWidget.cpp \
    EventHandling.cpp \
    Grid.cpp \
    HandleManager.cpp \
    main.cpp \
    MenuInit.cpp \
    MenuMain.cpp \
    MyCAD.cpp \
    Shape.cpp \
    ShapeCircle.cpp \
    ShapeLine.cpp \
    ShapeManager.cpp
