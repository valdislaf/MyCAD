#pragma once
#include "DrawingWidget.h"
#include "EventHandling.h"

extern int heightwindow_prev;

class MyCAD : public EventHandling
{
    Q_OBJECT

public:
    MyCAD(QWidget* parent = nullptr);
    ~MyCAD();
    QCursor createCustomCrossCursor();
    QCursor createCustomCrossCursorIn();
    QPen DashPen(QColor Color, qreal dashLength, qreal gapLength);
    QPen getDashPenForShape(const std::shared_ptr<Shape>& shape);
    void CrossCursorIn(QPainter& painter); // для перекрестия курсора активный 
    void CrossCursorOut(QPainter& painter); // для перекрестия курсора неактивный
    void DrawCircle(QPainter& painter, QPoint localPos);
    void drawGrid(QPainter& painter);
    void DrawLine(QPainter& painter, QPoint localPos);
    void drawLineToCurrentPoint(QPainter& painter, const std::shared_ptr<Shape>& shape);
    void drawShapes(QPainter& painter);
    void drawTemporaryShapes(QPainter& painter);
    void updateGridPosition(const QPoint& delta); // Метод для обновления позиции сетки

private slots:  // Методы, связанные с сигналами
    void onCloseThisTab();
    void onDrawCircle();
    void onDrawLine();
    void onExitThis();
    void onTabChanged(int index);

private:  // Обычные методы
    void createNewWindow();
    void setupTabWidgetStyle();
    void updateMenusBasedOnTabWidgetVisibility();
    QPoint offset;            // Смещение от начальной позиции
    QMenuBar* menuBar;
};
