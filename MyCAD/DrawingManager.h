#pragma once
#include <QPainter>
#include <QPoint>
#include <QColor>
#include <QPen>
#include "Grid.h"
#include "Shape.h"


class DrawingManager {
public:
    DrawingManager();

    void drawGrid(QPainter& painter, QWidget* currentTab, const QPoint& delta, int gridSize = 37);
    void drawLine(QPainter& painter, const QPoint& startPoint, const QPoint& endPoint, const QColor& color = QColor(255, 155, 155));
    void drawCircle(QPainter& painter, const QPoint& center, const QPoint& newpoint, const QColor& color = QColor(255, 155, 155));
    void drawCrossCursorIn(QPainter& painter, const QPoint& position);
    void drawCrossCursorOut(QPainter& painter, const QPoint& position);
   

//private:
    QPen createSolidPen(const QColor& color, int width = 1) const;
    QPen createDashPen(const QColor& color, qreal dashLength = 10, qreal gapLength = 5) const;
    void drawTemporaryShapes(QPainter& painter, std::vector<std::shared_ptr<Shape>>& tmpShapes, std::vector<std::shared_ptr<Shape>>& selShapes, const QPoint& newpoint);
    void drawLineToCurrentPoint(QPainter& painter, const std::shared_ptr<Shape>& shape, const QPoint& newpoint);
    QPen getDashPenForShape(const std::shared_ptr<Shape>& shape);
};
