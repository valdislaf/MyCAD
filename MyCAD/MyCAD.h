#pragma once

#include <memory>
#include <vector>
#include <QtWidgets/QMainWindow>

#include "Shape.h"
#include "DrawingWidget.h"

struct TabData {
    int delataX = 0;
    int delataY = 0;
    std::vector<std::shared_ptr<Shape>> shapes;  // Список фигур для этой вкладки
};
enum class DrawMode {
    None,
    Circle,
    Line
};
extern bool isdraw;
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
extern DrawMode currentDrawMode;
class MyCAD : public QMainWindow
{
    Q_OBJECT

public:
    MyCAD(QWidget* parent = nullptr);
    ~MyCAD();
    void drawShapes(QPainter& painter);          // Метод для рисования всех фигур    
    void drawGrid(QPainter& painter);
    void DrawLine(QPainter& painter, QPoint localPos);
    void DrawCircle(QPainter& painter, QPoint localPos);
    QPen DashPen(QColor Color, qreal dashLength, qreal gapLength);
    QCursor createCustomCrossCursor();
    QCursor createCustomCrossCursorIn();
    void CrossCursorIn(QPainter& painter); // для перекрестия курсора активный 
    void CrossCursorOut(QPainter& painter); // для перекрестия курсора неактивный

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void handleDrawing(QMouseEvent* event, bool& circleflag);
    void handleSelection(QMouseEvent* event, bool circleflag);
    void updateShapeCoordinates(int i);
    void resetShapeColors();
    void selectShapes(QMouseEvent* event);
    void highlightShapes(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool event(QEvent* e)override;
    void keyPressEvent(QKeyEvent* event) override;   

private slots:  // Методы, связанные с сигналами
    void onExitThis();
    void onCloseThisTab();
    void onDrawLine();
    void onDrawCircle();
    void onTabChanged(int index);

private:  // Обычные методы
    void createNewWindow();
    void captureCursorforHandle(QWidget* currentTab, const QRect& handleRect, const QPoint& cursorPos);
    void updateMenusBasedOnTabWidgetVisibility();
    void initialTabWidget();
    void setupTabWidgetStyle();
    void updateGridPosition(const QPoint& delta); // Метод для обновления позиции сетки
    void addShape(std::unique_ptr<Shape>&& shape);  // Метод для добавления фигуры
    void clearvectors();
    void clearSelection();
    void movingPush(HandleType handle, bool isselected);
    QVector<TabData> tabDataList; // Список данных для каждой вкладки
    bool isDragging = false;  // Флаг для отслеживания состояния перетаскивания
    QPoint lastMousePosition; // Последняя позиция мыши
    QPoint offset;            // Смещение от начальной позиции
    QTabWidget* tabWidget;
    QMenuBar* menuBar;
	
};
