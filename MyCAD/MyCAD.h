#pragma once

//#include <memory>
//#include <vector>
//#include <QtWidgets/QMainWindow>

#include "ShapeLine.h"
#include "ShapeCircle.h"

#include "DrawingWidget.h"
#include "EventHandling.h"
//#include <QTabWidget>
//#include <QMenuBar>

enum class DrawMode {
    None,
    Circle,
    Line
};

extern bool isdraw;
extern QPoint clickpoint;
extern int heightwindow_prev;
extern std::vector<bool> movingMiddles;
extern std::vector<bool>  movingEnds;
extern std::vector<bool>  movingStarts;
extern std::vector<bool> movingLefts;
extern std::vector<bool> movingTops;
extern std::vector<bool> movingRights;
extern std::vector<bool> movingBottoms;
extern std::vector<std::shared_ptr<Shape>>selShapes;
extern std::vector<std::shared_ptr<Shape>>tmpShapes;
extern DrawMode currentDrawMode;


class MyCAD : public EventHandling
{
    Q_OBJECT

public:
    MyCAD(QWidget* parent = nullptr);
    ~MyCAD();
    void drawShapes(QPainter& painter);
    void drawTemporaryShapes(QPainter& painter);
    void drawLineToCurrentPoint(QPainter& painter, const std::shared_ptr<Shape>& shape);
    // Метод для рисования всех фигур    
    QPen getDashPenForShape(const std::shared_ptr<Shape>& shape);
    void drawGrid(QPainter& painter);
    void DrawLine(QPainter& painter, QPoint localPos);
    void DrawCircle(QPainter& painter, QPoint localPos);
    QPen DashPen(QColor Color, qreal dashLength, qreal gapLength);
    QCursor createCustomCrossCursor();
    QCursor createCustomCrossCursorIn();
    void CrossCursorIn(QPainter& painter); // для перекрестия курсора активный 
    void CrossCursorOut(QPainter& painter); // для перекрестия курсора неактивный
    void updateGridPosition(const QPoint& delta); // Метод для обновления позиции сетки


protected:
    
 

private slots:  // Методы, связанные с сигналами
    void onExitThis();
    void onCloseThisTab();
    void onDrawLine();
    void onDrawCircle();
    void onTabChanged(int index);

private:  // Обычные методы
    void createNewWindow();

   
  //  void captureCursorforHandle(QWidget* currentTab, const QRect& handleRect, const QPoint& cursorPos);
    void updateMenusBasedOnTabWidgetVisibility();   
    void setupTabWidgetStyle();
    
    


   
   
   // QVector<TabData> tabDataList; // Список данных для каждой вкладки
  
    QPoint offset;            // Смещение от начальной позиции
   
    QMenuBar* menuBar;
	
};
