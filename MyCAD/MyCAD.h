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
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:  // Методы, связанные с сигналами
    void onExitThis();
    void onCloseThisTab();
    void onDrawLine();

private:  // Обычные методы
    void createNewWindow();
    void updateMenusBasedOnTabWidgetVisibility();
    void initialTabWidget();
    QCursor createCustomCrossCursor();
    void drawGrid();
    void setupTabWidgetStyle();
private:
    Ui::MyCADClass ui;
};
