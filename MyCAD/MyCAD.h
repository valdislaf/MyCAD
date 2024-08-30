#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MyCAD.h"

class MyCAD : public QMainWindow
{
    Q_OBJECT

public:
    MyCAD(QWidget *parent = nullptr);
    ~MyCAD();


private slots:
    void onExitThis();
    void onDrawLine();
    void createNewWindow();
    void updateMenusBasedOnTabWidgetVisibility();
    void initialTabWidget();
private:
    Ui::MyCADClass ui;
};
