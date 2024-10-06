#pragma once
#include <QMenuBar>
#include <QAction>
#include <QTabBar>
#include <QTabWidget>
#include <QWidget>
#include <QMenuBar>

class MenuMain : public QWidget {

    Q_OBJECT

public:
    MenuMain(QMenuBar* menuBar);
    ~MenuMain();

public:
    QAction* GetNewWindowAction();
    QAction* GetCloseWindowAction();
    QAction* GetexitAction();
    QAction* GetlineAction();
    QAction* GetcircleAction();

private:
    QMenuBar* menuBar;
    QMenu* FileMenu;
    QAction* NewWindowAction;//**
    QAction* CloseWindowAction;//**
    QAction* exitAction; //**
    QMenu* EditMenu;
    QMenu* ViewMenu;
    QMenu* InsertMenu;
    QMenu* FormatMenu;
    QMenu* ServiceMenu;
    QMenu* DrawMenu;
    QMenu* modelingSubMenu;
    QAction* lineAction;//**
    QMenu* modecircleSubMenu;
    QAction* circleAction;//**
    QAction* circleAction2;
    QMenu* DimensionMenu ;
    QMenu* EditMenu2;
    QMenu* ParameterizationMenu;
    QMenu* WindowMenu;
    QMenu* HelpMenu;
};
