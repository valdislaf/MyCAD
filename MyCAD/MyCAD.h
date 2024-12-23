#pragma once
#include "DrawingWidget.h"
#include "EventHandling.h"
#include "DrawingManager.h"

extern int heightwindow_prev;

extern bool disableCursor;
class MyCAD : public EventHandling
{
    Q_OBJECT

public:
    MyCAD(QWidget* parent = nullptr);
    ~MyCAD();
   
    void CrossCursorIn(QPainter& painter); // для перекрестия курсора активный  
    void CrossCursorOut(QPainter& painter); // для перекрестия курсора неактивный
    void CrossCursorHandle(QPainter& painter);// для перекрестия курсора внутри handle 
    void DrawCircle(QPainter& painter, QPoint localPos);    
    bool isDrawEnabled() const;
    bool isTabActive() const;
    void drawGrid(QPainter& painter);
    void DrawLine(QPainter& painter, QPoint localPos);  
    void drawShapes(QPainter& painter);  
    void updateGridPosition(const QPoint& delta); // Метод для обновления позиции сетки

    

private slots:  // Методы, связанные с сигналами
    void onCloseThisTab();
    void onDrawCircle();
    void onDrawLine();
    void onExitThis();
    void onTabChanged();

private:  // Обычные методы
    void createNewWindow();
    void setupTabWidgetStyle();
    void updateMenusBasedOnTabWidgetVisibility();
    QPoint offset;            // Смещение от начальной позиции
    QMenuBar* menuBar;
    DrawingManager* drawingManager;
   
};
