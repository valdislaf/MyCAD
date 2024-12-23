#pragma once
#include <QWidget>
#include <QPainter>

class Grid {
public:
    Grid(QWidget* currentTab, int gridSize, int delataX, int delataY); // Конструктор принимает QWidget*
    int setDelataX(int dx);
    int setDelataY(int dy);
    void setGridSize(int size);
    void setTab(QWidget* currTab);
    void draw(QPainter& painter); // Метод для рисования сетки
    void CoordinateAxes(QPainter& painter, QWidget* currentTab);
   

private:
    QWidget* currentTab;  // Ссылка на виджет
    int gridSize;         // Размер ячейки сетки
    int delataX;          // Смещение по X
    int delataY;          // Смещение по Y
   
};
