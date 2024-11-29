#include "DrawingManager.h"
#include <cmath>

DrawingManager::DrawingManager() {}

void DrawingManager::drawGrid(QPainter& painter, QWidget* currentTab, const QPoint& delta, int gridSize) {
    Grid grid(currentTab, gridSize, delta.x(), delta.y());
    grid.draw(painter);
}

void DrawingManager::drawLine(QPainter& painter, const QPoint& startPoint, const QPoint& endPoint, const QColor& color) {
    painter.setPen(createSolidPen(color));
    painter.drawLine(startPoint, endPoint);
}

void DrawingManager::drawCircle(QPainter& painter, const QPoint& center, const QPoint& newpoint, const QColor& color) {
    int radius = std::hypot(newpoint.x() - center.x(), newpoint.y() - center.y());
    painter.setPen(createSolidPen(color));
    painter.drawEllipse(center, radius, radius);
}

void DrawingManager::drawCrossCursorIn(QPainter& painter, const QPoint& position) {
    painter.drawLine(position.x() - 48, position.y(), position.x() + 48, position.y());
    painter.drawLine(position.x(), position.y() - 48, position.x(), position.y() + 48);
}

void DrawingManager::drawCrossCursorOut(QPainter& painter, const QPoint& newpoint) {
    painter.drawLine(newpoint.x() - 3, newpoint.y() - 3, newpoint.x() + 3, newpoint.y() - 3);
    painter.drawLine(newpoint.x() - 3, newpoint.y() + 3, newpoint.x() + 3, newpoint.y() + 3);

    // Вертикальные линии
    painter.drawLine(newpoint.x() - 3, newpoint.y() - 3, newpoint.x() - 3, newpoint.y() + 3);
    painter.drawLine(newpoint.x() + 3, newpoint.y() - 3, newpoint.x() + 3, newpoint.y() + 3);

    int cursorSize = 97;
    int squareSide = 3; // Половина стороны квадрата 6x6

    // Вертикальные линии перекрестия
    painter.drawLine(newpoint.x(), newpoint.y() - cursorSize / 2, newpoint.x(), newpoint.y() - squareSide); // Вверх
    painter.drawLine(newpoint.x(), newpoint.y() + squareSide, newpoint.x(), newpoint.y() + cursorSize / 2); // Вниз

    // Горизонтальные линии перекрестия
    painter.drawLine(newpoint.x() - cursorSize / 2, newpoint.y(), newpoint.x() - squareSide, newpoint.y()); // Влево
    painter.drawLine(newpoint.x() + squareSide, newpoint.y(), newpoint.x() + cursorSize / 2, newpoint.y()); // Вправо
}

QPen DrawingManager::createSolidPen(const QColor& color, int width) const {
    return QPen(color, width, Qt::SolidLine);
}

QPen DrawingManager::createDashPen(const QColor& color, qreal dashLength, qreal gapLength) const {
    QPen pen(color, 1);
    pen.setDashPattern({ dashLength, gapLength });
    return pen;
}

void DrawingManager::drawTemporaryShapes(
    QPainter& painter, std::vector<std::shared_ptr<Shape>>&tmpShapes, 
    std::vector<std::shared_ptr<Shape>>& selShapes, const QPoint& newpoint) {
    auto tmpShapeIt = tmpShapes.begin();
    for (auto& shape : selShapes) {
        if (tmpShapeIt != tmpShapes.end()) {
            QPen pen = painter.pen();
            painter.setPen(getDashPenForShape(*tmpShapeIt));
            drawLineToCurrentPoint(painter, *tmpShapeIt, newpoint);
            painter.setPen(pen);  // Возвращаем Pen
            shape->draw(painter);
            ++tmpShapeIt;
        }
    }
}

// Метод для рисования линии к текущей точке
void DrawingManager::drawLineToCurrentPoint(QPainter& painter, const std::shared_ptr<Shape>& shape, const QPoint& newpoint) {
  
    if (shape->getisStart()) {
        painter.drawLine(shape->getstartPoint(), newpoint);
    }

    else if (shape->getisEnd()) {
        painter.drawLine(shape->getendPoint(), newpoint);
    }
    else if (shape->getisMiddle()) {
        painter.drawLine(shape->getmiddlePoint(), newpoint);
    }
    else if (shape->getisLeft() || shape->getisTop() || shape->getisRight() || shape->getisBottom()) {
        painter.drawLine(shape->getstartPoint(), newpoint);
    }
}

// Метод для получения DashPen в зависимости от состояния фигуры
QPen DrawingManager::getDashPenForShape(const std::shared_ptr<Shape>& shape) {
    if (shape->getisLeft() || shape->getisTop() || shape->getisRight() || shape->getisBottom()) {
        return createDashPen(QColor(161, 161, 161), 2, 2);
    }
    else {
        return createDashPen(QColor(212, 161, 32), 10, 5);
    }
}

