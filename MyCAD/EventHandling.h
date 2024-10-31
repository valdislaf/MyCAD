#pragma once
#include "CrossCursor.h"
#include "Grid.h"
#include "MenuInit.h"
#include "MenuMain.h"
#include "ShapeCircle.h"
#include "ShapeLine.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>

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

extern DrawMode currentDrawMode;
extern bool isdraw;
extern QPoint clickpoint;
extern std::vector<bool>  movingEnds;
extern std::vector<bool>  movingStarts;
extern std::vector<bool> movingLefts;
extern std::vector<bool> movingTops;
extern std::vector<bool> movingRights;
extern std::vector<bool> movingBottoms;
extern std::vector<std::shared_ptr<Shape>>selShapes;
extern std::vector<std::shared_ptr<Shape>>tmpShapes;

class EventHandling : public QMainWindow {
    Q_OBJECT

public:
    EventHandling(QWidget* parent = nullptr);
    ~EventHandling();

protected:
    bool chekTab();
    bool event(QEvent* e) override;
    bool isDragging = false;
    bool shapesNoEmpt();
    QPoint GetCurrPoint();
    QPoint lastMousePosition;
    QTabWidget* tabWidget;
    QVector<TabData> tabDataList; // Список данных для каждой вкладки
    void addShape(std::unique_ptr<Shape>&& shape);
    void clearSelection();
    void clearvectors();
    void handleDrawing(QMouseEvent* event, bool& circleFlag);
    void handleHoverMoveEvent();
    void handleSelection(QMouseEvent* event, bool circleFlag);
    void highlightShapes(QMouseEvent* event);
    void highlightShapesUnderCursor(int currentIndex);
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void movingPush(HandleType handle, bool isselected);
    void processShapeSelection(int currentIndex);
    void resetShapeColors();
    void selectShapes(QMouseEvent* event);
    void updateGridPosition(const QPoint& delta);
    void updateShapeCoordinates(int i);
    void updateShapePositions();
};
