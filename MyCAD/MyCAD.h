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
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    int delataX = 10;
    int delataY = -10;

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

private:
        bool isDragging = false;  // Флаг для отслеживания состояния перетаскивания
        QPoint lastMousePosition; // Последняя позиция мыши
        QPoint offset;            // Смещение от начальной позиции

        void updateGridPosition(const QPoint& delta); // Метод для обновления позиции сетки
       
};
