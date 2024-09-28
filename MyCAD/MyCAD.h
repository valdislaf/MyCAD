#pragma once

#include <memory>
#include <vector>

#include <QtWidgets/QMainWindow>

#include "Shape.h"
#include "ui_MyCAD.h"
#include "DrawingWidget.h"

struct TabData {
    int delataX = 0;
    int delataY = 0;
    std::vector<std::shared_ptr<Shape>> shapes;  // Список фигур для этой вкладки
};
extern bool isdraw;
extern bool ondrawline;
extern bool ondrawcircle;
extern QPoint clickpoint;


extern std::vector<bool> movingWholeLines;
extern std::vector<bool>  movingEnds;
extern std::vector<bool>  movingStarts;

extern std::vector<std::shared_ptr<Shape>>selShapes;
extern std::vector<std::shared_ptr<Shape>>tmpShapes;

class MyCAD : public QMainWindow
{
    Q_OBJECT

public:
    MyCAD(QWidget* parent = nullptr);
    ~MyCAD();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool event(QEvent* e)override;
    void keyPressEvent(QKeyEvent* event);
private:
    QVector<TabData> tabDataList; // Список данных для каждой вкладки

private slots:  // Методы, связанные с сигналами
    void onExitThis();
    void onCloseThisTab();
    void onDrawLine();
    void onDrawCircle();
    void onTabChanged(int index);

private:  // Обычные методы
    void createNewWindow();
    void updateMenusBasedOnTabWidgetVisibility();
    void initialTabWidget();
    void setupTabWidgetStyle();
    void CoordinateAxes(QPainter& painter, QWidget* currentTab);
    void updateGridPosition(const QPoint& delta); // Метод для обновления позиции сетки
    void addShape(std::unique_ptr<Shape>&& shape);  // Метод для добавления фигуры

private:
    Ui::MyCADClass ui;

private:
    bool isDragging = false;  // Флаг для отслеживания состояния перетаскивания
    QPoint lastMousePosition; // Последняя позиция мыши
    QPoint offset;            // Смещение от начальной позиции

public:
    void drawShapes(QPainter& painter);          // Метод для рисования всех фигур
    
    void drawGrid(QPainter& painter);
    void DrawLine(QPainter& painter, QPoint localPos);
    QCursor createCustomCrossCursor();
    QCursor createCustomCrossCursorIn();
};
