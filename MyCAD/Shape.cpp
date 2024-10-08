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

void Line::setCentre(const QPoint& startPoint, const int radius, bool isSelected)
{
}

QPoint Line::getstartPoint() const
{
    return startPoint;
}

QPoint Line::getendPoint() const
{
    return endPoint;
}

QPoint Line::getleftPoint() const
{
    return QPoint();
}

QPoint Line::gettopPoint() const
{
    return QPoint();
}

QPoint Line::getrightPoint() const
{
    return QPoint();
}

QPoint Line::getbottomPoint() const
{
    return QPoint();
}

int Line::getradius() const
{
    return 0;
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

bool Line::getisStart() const
{
    return isStart;
}

bool Line::getisEnd() const
{
    return isEnd;
}

bool Line::getisMiddle() const
{
    return isMiddle;
}

bool Line::getisLeft() const
{
    return false;
}

bool Line::getisRight() const
{
    return false;
}

bool Line::getisTop() const
{
    return false;
}

bool Line::getisBottom() const
{
    return false;
}

Type Line::name() const
{
    return Type::line;
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
       /* painter.setPen(painter.pen());
        painter.drawLine(startPoint, endPoint);*/
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
        // Отключаем заливку
        painter.setBrush(Qt::NoBrush);
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

void Line::moveRadius(const int radius)
{
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

Circle::Circle(const QPoint& startPoint, const int radius)
    : startPoint(startPoint), radius(radius) {}

Circle::Circle(const Circle& other)
    : startPoint(other.startPoint), radius(other.radius) {
}

Circle::~Circle()
{
}

void Circle::draw(QPainter& painter) const
{
    QPen pen = painter.pen();
    // Если линия выделена, изменяем цвет
    if (isSelected) {
        QColor highlightedColor(90, 150, 255, 169);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawEllipse(startPoint, radius, radius);
        
    }

    painter.setPen(pen);
    painter.drawEllipse(startPoint, radius, radius);

    // Рисуем квадраты на сверху снизу спарва и слева, если линия выделена
    if (isSelected) {

        painter.setBrush(ColorStartPoint);
        painter.drawRect(getStartHandle());       

        painter.setBrush(ColorLeftPoint);
        painter.drawRect(getLeftHandle());

        painter.setBrush(ColorTopPoint);
        painter.drawRect(getToptHandle());

        painter.setBrush(ColorRightPoint);
        painter.drawRect(getRighttHandle());

        painter.setBrush(ColorBottomPoint);
        painter.drawRect(getBottomtHandle());

        // Отключаем заливку
        painter.setBrush(Qt::NoBrush);
    }
}

void Circle::move(const QPoint& delta)
{
    startPoint += delta;
}

void Circle::moveStart(const QPoint& delta)
{
    startPoint += delta;
}

void Circle::moveEnd(const QPoint& delta)
{

}

void Circle::moveRadius(const int radius_)
{
    radius = radius_;
}

bool Circle::contains(const QPoint& point) const
{
    // Вычисляем расстояние от точки до центра
    double distanceSquared = pow(point.x() - startPoint.x(), 2) + pow(point.y() - startPoint.y(), 2);
    double radiusSquared = radius * radius;
    //float  result = fabs(distanceSquared - radiusSquared) ;
    // Проверяем условие на нахождение точки на окружности с учетом погрешности
    return fabs(distanceSquared - radiusSquared) < 1000;
}

void Circle::setSelected(bool selected)
{
    isSelected = selected;
}

HandleType Circle::getHandleAt(const QPoint& point)
{
    
    ColorStartPoint = QColor(0, 127, 255);
    ColorLeftPoint = QColor(0, 127, 255);
     ColorTopPoint = QColor(0, 127, 255);
     ColorRightPoint = QColor(0, 127, 255);
     ColorBottomPoint = QColor(0, 127, 255);

    if (getStartHandle().contains(point)) {
        ColorStartPoint = QColor(0, 0, 0);
        isStart = true;
        return HandleType::StartHandle;
    }

    if (getLeftHandle().contains(point)) {
        ColorLeftPoint = QColor(165, 0, 0);
        isLeft = true;
        return HandleType::LeftHandle;
    }

    if (getToptHandle().contains(point)) {
        ColorTopPoint = QColor(165, 0, 0);
        isTop = true;
        return HandleType::TopHandle;
    }

    if (getRighttHandle().contains(point)) {
        ColorRightPoint = QColor(165, 0, 0);
        isRight = true;
        return HandleType::RightHandle;
    }

    if (getBottomtHandle().contains(point)) {
        ColorBottomPoint = QColor(165, 0, 0);
        isBottom = true;
        return HandleType::BottomHandle;
    }

    return HandleType::None;
}

std::shared_ptr<Shape> Circle::clone() const
{    
    return std::make_shared<Circle>(*this);
}

void Circle::setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected)
{
    
}

void Circle::setCentre(const QPoint& startPoint_, const int radius_, bool isSelected_)
{
    startPoint = startPoint_;  radius = radius_; isSelected = isSelected_;
}

QPoint Circle::getstartPoint() const
{
    return startPoint;
}

QPoint Circle::getendPoint() const
{
    return QPoint();
}

QPoint Circle::getleftPoint() const
{
    return leftPoint;
}

QPoint Circle::gettopPoint() const
{
    return topPoint;
}

QPoint Circle::getrightPoint() const
{
    return rightPoint;
}

QPoint Circle::getbottomPoint() const
{
    return bottomPoint;
}

int Circle::getradius() const
{
    return radius;
}

bool Circle::getisSelected() const
{
    return isSelected;
}

void Circle::resetColor()
{
     ColorStartPoint = QColor(0, 127, 255);
     ColorLeftPoint = QColor(0, 127, 255);
     ColorTopPoint = QColor(0, 127, 255);
     ColorRightPoint = QColor(0, 127, 255);
     ColorBottomPoint = QColor(0, 127, 255);
     isStart = false;  // Флаг выделения
     isLeft = false;  // Флаг выделения
     isTop = false;  // Флаг выделения
     isRight = false;  // Флаг выделения
     isBottom = false;  // Флаг выделения
}

bool Circle::getisStart()const
{
    return isStart;
}

bool Circle::getisEnd()const
{
    return false;
}

bool Circle::getisMiddle()const
{
    return false;
}

bool Circle::getisLeft()const
{
    return isLeft;
}

bool Circle::getisRight()const
{
    return isRight;
}

bool Circle::getisTop() const
{
    return isTop;
}

bool Circle::getisBottom() const
{
    return isBottom;
}

Type Circle::name() const
{
    return Type::circle;
}

QRect Circle::getStartHandle() const
{
    return QRect(startPoint.x() - handleSize / 2, startPoint.y() - handleSize / 2, handleSize, handleSize);
}

QRect Circle::getLeftHandle() const {
    // Центр окружности: centerPoint, радиус: radius
    return QRect(startPoint.x() - radius - handleSize / 2, startPoint.y() - handleSize / 2, handleSize, handleSize);
}

QRect Circle::getRighttHandle() const {
    return QRect(startPoint.x() + radius - handleSize / 2, startPoint.y() - handleSize / 2, handleSize, handleSize);
}

QRect Circle::getToptHandle() const {
    return QRect(startPoint.x() - handleSize / 2, startPoint.y() - radius - handleSize / 2, handleSize, handleSize);
}

QRect Circle::getBottomtHandle() const {
    return QRect(startPoint.x() - handleSize / 2, startPoint.y() + radius - handleSize / 2, handleSize, handleSize);
}


