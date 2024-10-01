#include "Shape.h"
#include <QDebug>

// Реализация конструктора для класса Line
Line::Line(const QPoint& startPoint, const QPoint& endPoint)
    : startPoint(startPoint), endPoint(endPoint) {}

Line::Line(const Line& other)
    : startPoint(other.startPoint), endPoint(other.endPoint) {}

Line::~Line()
{
}


// Реализация метода  клонирования
std::shared_ptr<Shape> Line::clone() const  {
    return std::make_shared<Line>(*this);
}

void Line::setCoords(const QPoint& startPoint_, const QPoint& endPoint_, bool isSelected_) 
{
    startPoint = startPoint_; endPoint = endPoint_; isSelected = isSelected_;
}

QPoint Line::getstartPoint() const
{
    return startPoint;
}

QPoint Line::getendPoint() const
{
    return endPoint;
}

bool Line::getisSelected() const
{
    return isSelected;
}

void Line::resetColor()
{
    ColorStartPoint = QColor(0, 127, 255);
    ColorEndPoint = QColor(0, 127, 255);
    ColorMiddlePoint = QColor(0, 127, 255);
     isStart = false;  // Флаг выделения
     isEnd = false;  // Флаг выделения
     isMiddle = false;  // Флаг выделения
}

bool Line::getisStart()
{
    return isStart;
}

bool Line::getisEnd()
{
    return isEnd;
}

bool Line::getisMiddle()
{
    return isMiddle;
}


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
        painter.setBrush(ColorStartPoint); 
        painter.drawRect(getStartHandle());
        painter.setBrush(ColorEndPoint);
        painter.drawRect(getEndHandle());
        painter.setBrush(ColorMiddlePoint);
        painter.drawRect(getMiddleHandle());
    }
}

// Реализация метода перемещения отрезка
void Line::move(const QPoint& delta) {
    startPoint += delta;
    endPoint += delta;
}

void Line::moveStart(const QPoint& delta)
{
    startPoint += delta;
}

void Line::moveEnd(const QPoint& delta)
{
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

HandleType Line::getHandleAt(const QPoint& point)  {

    QRect start = getStartHandle();
    QRect end = getEndHandle();
    QRect middle = getMiddleHandle();

    ColorStartPoint = QColor(0, 127, 255);
    ColorEndPoint = QColor(0, 127, 255);
    ColorMiddlePoint = QColor(0, 127, 255);

    if (getStartHandle().contains(point)) {
         ColorStartPoint = QColor(0, 0, 0);
         isStart = true;
        return HandleType::StartHandle;
    }

    if (getEndHandle().contains(point)) {
         ColorEndPoint = QColor(165, 0, 0);
         isEnd = true;
        return HandleType::EndHandle;
    }

    if (getMiddleHandle().contains(point)) {
         ColorMiddlePoint = QColor(165, 0, 0);
         isMiddle = true;
        return HandleType::MiddleHandle;
    }

    return HandleType::None;
}

Circle::Circle(const QPoint& startPoint, const QPoint& endPoint) 
    : startPoint(startPoint), endPoint(endPoint) {}

Circle::Circle(const Circle& other)
    : startPoint(other.startPoint), endPoint(other.endPoint) {
}

Circle::~Circle()
{
}

void Circle::draw(QPainter& painter) const
{
}

void Circle::move(const QPoint& delta)
{
}

void Circle::moveStart(const QPoint& delta)
{
}

void Circle::moveEnd(const QPoint& delta)
{
}

bool Circle::contains(const QPoint& point) const
{
    return false;
}

void Circle::setSelected(bool selected)
{
}

HandleType Circle::getHandleAt(const QPoint& point)
{
    return HandleType();
}

std::shared_ptr<Shape> Circle::clone() const
{
    return std::shared_ptr<Shape>();
}

void Circle::setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected)
{
}

QPoint Circle::getstartPoint() const
{
    return QPoint();
}

QPoint Circle::getendPoint() const
{
    return QPoint();
}

bool Circle::getisSelected() const
{
    return false;
}

void Circle::resetColor()
{
}

bool Circle::getisStart()
{
    return false;
}

bool Circle::getisEnd()
{
    return false;
}

bool Circle::getisMiddle()
{
    return false;
}

QRect Circle::getStartHandle() const
{
    return QRect();
}

QRect Circle::getEndHandle() const
{
    return QRect();
}

QRect Circle::getMiddleHandle() const
{
    return QRect();
}
