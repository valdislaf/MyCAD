#pragma once
#include <QMenuBar>
#include <QAction>

class Menu {

public:
    Menu(QMenuBar* menuBar);

private:
    QMenuBar* menuBar;
};
