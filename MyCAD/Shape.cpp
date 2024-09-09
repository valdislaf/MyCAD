#include "Shape.h"
#include <QDebug>

// Реализация конструктора для класса Line
Line::Line(const QPoint& startPoint, const QPoint& endPoint)
    : startPoint(startPoint), endPoint(endPoint) {}

// Реализация метода рисования отрезка
void Line::draw(QPainter& painter) const {
    QPen pen = painter.pen();

    // Если линия выделена, изменяем цвет
    if (isSelected) {
        QColor highlightedColor(90, 150, 255, 169);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawLine(startPoint, endPoint);
        painter.setPen(painter.pen());
        painter.drawLine(startPoint, endPoint);
    }

    painter.setPen(pen);
    painter.drawLine(startPoint, endPoint);

    // Рисуем квадраты на концах и в середине, если линия выделена
    if (isSelected) {
        painter.setBrush(Qt::blue);  // Голубая заливка для квадратов
        painter.drawRect(getStartHandle());
        painter.drawRect(getEndHandle());
        painter.drawRect(getMiddleHandle());
    }
}

// Реализация метода перемещения отрезка
void Line::move(const QPoint& delta) {
    startPoint += delta;
    endPoint += delta;
}

// Метод установки состояния выделения
void Line::setSelected(bool selected) {
    isSelected = selected;
}

// Реализация метода проверки попадания точки в отрезок
bool Line::contains(const QPoint& point) const {
    // Проверка попадания в квадраты (для перемещения концов или всей линии)
    if (getStartHandle().contains(point) || getEndHandle().contains(point) || getMiddleHandle().contains(point)) {
        return true;
    }

    // Основная логика проверки принадлежности точки линии
    QPoint v1 = point - startPoint;
    QPoint v2 = endPoint - startPoint;

    double lengthSquared = v2.x() * v2.x() + v2.y() * v2.y();
    double dotProduct = v1.x() * v2.x() + v1.y() * v2.y();

    double projection = dotProduct / lengthSquared;

    if (projection < 0 || projection > 1) {
        return false;
    }

    QPoint closestPoint = startPoint + projection * v2;
   // if ((point - closestPoint).manhattanLength() < 5) { setSelected(true); }
    return (point - closestPoint).manhattanLength() < 5; // Допускаем небольшое расстояние для попадания
}

// Возвращает область квадрата в начале линии
QRect Line::getStartHandle() const {
    return QRect(startPoint.x() - handleSize / 2, startPoint.y() - handleSize / 2, handleSize, handleSize);
}

// Возвращает область квадрата в конце линии
QRect Line::getEndHandle() const {
    return QRect(endPoint.x() - handleSize / 2, endPoint.y() - handleSize / 2, handleSize, handleSize);
}

// Возвращает область квадрата в середине линии
QRect Line::getMiddleHandle() const {
    QPoint middlePoint = (startPoint + endPoint) / 2;
    return QRect(middlePoint.x() - handleSize / 2, middlePoint.y() - handleSize / 2, handleSize, handleSize);
}

HandleType Line::getHandleAt(const QPoint& point) const {

    QRect start = getStartHandle();
    QRect end = getEndHandle();
    QRect middle = getMiddleHandle();

    if (getStartHandle().contains(point)) {
        
        return HandleType::StartHandle;
    }
    if (getEndHandle().contains(point)) {
       
        return HandleType::EndHandle;
    }
    if (getMiddleHandle().contains(point)) {
        
        return HandleType::MiddleHandle;
    }
    return HandleType::None;
}
