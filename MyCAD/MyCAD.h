#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MyCAD.h"

class MyCAD : public QMainWindow
{
    Q_OBJECT

public:
    MyCAD(QWidget *parent = nullptr);
    ~MyCAD();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onExitThis();
    void onCloseThisTab();
    void onDrawLine();
    void createNewWindow();
    void updateMenusBasedOnTabWidgetVisibility();
    void initialTabWidget();
private:
    Ui::MyCADClass ui;
};
