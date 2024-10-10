#pragma once

#include <memory>
#include <vector>

#include <QtWidgets/QMainWindow>

#include "Shape.h"
//#include "ui_MyCAD.h"
#include "DrawingWidget.h"

struct TabData {
    int delataX = 0;
    int delataY = 0;
    std::vector<std::shared_ptr<Shape>> shapes;  // Список фигур для этой вкладки
};

extern bool isdraw;
extern bool ondrawline;
extern bool ondrawcircle;
extern bool updrawcircle;
extern QPoint clickpoint;
extern int heightwindow_prev;

extern std::vector<bool> movingWholeLines;
extern std::vector<bool>  movingEnds;
extern std::vector<bool>  movingStarts;

extern std::vector<bool> movingLefts;
extern std::vector<bool> movingTops;
extern std::vector<bool> movingRights;
extern std::vector<bool> movingBottoms;

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
    void movingPush(HandleType handle, bool isselected);

private:  // Обычные методы
    void createNewWindow();
    void updateMenusBasedOnTabWidgetVisibility();
    void initialTabWidget();
    void setupTabWidgetStyle();
    void CoordinateAxes(QPainter& painter, QWidget* currentTab);
    void updateGridPosition(const QPoint& delta); // Метод для обновления позиции сетки
    void addShape(std::unique_ptr<Shape>&& shape);  // Метод для добавления фигуры
    void clearSelection();
private:
    //Ui::MyCADClass ui;

private:
    bool isDragging = false;  // Флаг для отслеживания состояния перетаскивания
    QPoint lastMousePosition; // Последняя позиция мыши
    QPoint offset;            // Смещение от начальной позиции
    QTabWidget* tabWidget;
    QMenuBar* menuBar;

public:
    void drawShapes(QPainter& painter);          // Метод для рисования всех фигур
    
    void drawGrid(QPainter& painter);
    void DrawLine(QPainter& painter, QPoint localPos);
    void DrawCircle(QPainter& painter, QPoint localPos);
    QCursor createCustomCrossCursor();
    QCursor createCustomCrossCursorIn();
};
