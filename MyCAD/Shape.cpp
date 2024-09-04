#include "Shape.h"
#include <QDebug>

// Реализация конструктора для класса Line
Line::Line(const QPoint& startPoint, const QPoint& endPoint)
    : startPoint(startPoint), endPoint(endPoint) {}

// Реализация метода рисования отрезка
void Line::draw(QPainter& painter) const {
    qDebug() << "startPoint:" << startPoint;
    qDebug() << "endPoint:" << endPoint;
    painter.drawLine(startPoint, endPoint);
}

// Реализация метода перемещения отрезка
void Line::move(const QPoint& delta) {
    startPoint += delta;
    endPoint += delta;
}

// Реализация метода проверки попадания в отрезок
bool Line::contains(const QPoint& point) const {
    // Логика проверки принадлежности точки линии (можно использовать расстояние до линии)
    return false; // Пока заглушка
}
