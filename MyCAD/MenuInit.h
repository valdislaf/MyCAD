#pragma once
#include <QMenuBar>
#include <QAction>
#include <QTabBar>
#include <QTabWidget>
#include <QWidget>
#include <QMenuBar>

class MenuInit : public QWidget {

    Q_OBJECT

public:
    MenuInit(QMenuBar* menuBar);
    ~MenuInit();

public:
    QAction* GetNewWindowAction();
    QAction* GetCloseWindowAction();
    QAction* GetexitAction();

private:
    QMenuBar* menuBar;
    QMenu* FileMenu;
    QAction* NewWindowAction;//
    QAction* CloseWindowAction;//
    QAction* exitAction; //
    QMenu* WindowMenu;
    QMenu* HelpMenu;
};
