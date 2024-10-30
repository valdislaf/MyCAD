#pragma once


#include "Shape.h"

//#include <QTabWidget>
//#include <QMenuBar>

#include <QMainWindow>
//#include <QMouseEvent>
//#include <QKeyEvent>
//#include <QEvent>
//#include <QPainter>

struct TabData {
    int delataX = 0;
    int delataY = 0;
    std::vector<std::shared_ptr<Shape>> shapes;  // Список фигур для этой вкладки
};

class EventHandling : public QMainWindow {
    Q_OBJECT

public:
    EventHandling(QWidget* parent = nullptr);
    ~EventHandling();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    bool event(QEvent* e) override;

    bool isDragging = false;
    QPoint lastMousePosition;
    QVector<TabData> tabDataList; // Список данных для каждой вкладки
    QTabWidget* tabWidget;

    void handleHoverMoveEvent();
    void handleDrawing(QMouseEvent* event, bool& circleFlag);
    void handleSelection(QMouseEvent* event, bool circleFlag);
    void updateShapeCoordinates(int i);
    void resetShapeColors();
    QPoint GetCurrPoint();
    void selectShapes(QMouseEvent* event);
    void highlightShapes(QMouseEvent* event);
    void processShapeSelection(int currentIndex);
    void highlightShapesUnderCursor(int currentIndex);
    void updateShapePositions();
    void updateGridPosition(const QPoint& delta);
    bool chekTab();
    void movingPush(HandleType handle, bool isselected);
    bool shapesNoEmpt();
    void addShape(std::unique_ptr<Shape>&& shape);
    void clearvectors();
    void clearSelection();
};
