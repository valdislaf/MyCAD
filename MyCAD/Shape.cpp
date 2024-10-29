#include "Shape.h"
#include <QDebug>
#include <QTabWidget>
#include <QWidget>

// Реализация конструктора для класса Line
Line::Line(const QPoint& startPoint, const QPoint& endPoint)
   // : startPoint(startPoint), endPoint(endPoint) 
{
    handleManager = new HandleManager(10);
    ShapePoints points;
    points.startPoint = startPoint;
    points.endPoint = endPoint;
    setPoints(points);
}

Line::Line(const Line& other)
    //: startPoint(other.startPoint), endPoint(other.endPoint)
{
    handleManager = new HandleManager(10);
    const  ShapePoints& otherpoints = other.getPoints();
    ShapePoints points;
    points.startPoint = otherpoints.startPoint;
    points.endPoint = otherpoints.endPoint;
    setPoints(points);
}

Line::~Line()
{
    delete handleManager;
}


// Реализация метода  клонирования
std::shared_ptr<Shape> Line::clone() const  {
    return std::make_shared<Line>(*this);
}

void Line::setCoords(const QPoint& startPoint_, const QPoint& endPoint_, bool isSelected_) 
{
   // startPoint = startPoint_; endPoint = endPoint_; 
    isSelected = isSelected_;
    ShapePoints points;
    points.startPoint = startPoint_;
    points.endPoint = endPoint_;
    setPoints(points);
}

void Line::setCentre(const QPoint& startPoint, const int radius, bool isSelected)
{
}

QPoint Line::getstartPoint() const
{
    return getPoints().startPoint;
}

QPoint Line::getendPoint() const
{
    return getPoints().endPoint;
}

QPoint Line::getmiddlePoint()const
{
    const auto& points = getPoints();
   // middlePoint = (getPoints().startPoint + getPoints().endPoint) / 2;
    return (points.startPoint + points.endPoint) / 2;
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

void Line::setisover(bool iscursorhovershape_) 
{
     iscursorhovershape = iscursorhovershape_;
}

bool Line::getisover() const
{
    return iscursorhovershape;
}


// Реализация метода рисования отрезка
void Line::draw(QPainter& painter) const {
    QPen pen = painter.pen();
    const auto& points = getPoints();
    // Если линия выделена, изменяем цвет
    if (isSelected && !moveSelected) {
        QColor highlightedColor(90, 150, 255, 169);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawLine(points.startPoint, points.endPoint);
        // возвращаем Pen
        painter.setPen(pen);
        // Рисуем квадраты на концах и в середине, если линия выделена
        painter.setBrush(ColorStartPoint);
        painter.drawRect(getStartHandle());
        painter.setBrush(ColorEndPoint);
        painter.drawRect(getEndHandle());
        painter.setBrush(ColorMiddlePoint);
        painter.drawRect(getMiddleHandle());
        // Отключаем заливку
        painter.setBrush(Qt::NoBrush);
    }

     if (iscursorhovershape) {
        QColor highlightedColor(250, 250, 250, 120);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawLine(points.startPoint, points.endPoint);

        // возвращаем Pen
        painter.setPen(pen);
    }
    
    painter.setPen(pen);
    painter.drawLine(points.startPoint, points.endPoint);
   
    
}

// Реализация метода перемещения отрезка
void Line::move(const QPoint& delta) {
    auto points = getPoints();
    points.startPoint += delta;
    points.endPoint += delta;
    setPoints(points);
    
}

void Line::moveStart(const QPoint& delta)
{
    auto points = getPoints();
    points.startPoint += delta; 
    setPoints(points);
}

void Line::moveEnd(const QPoint& delta)
{
    auto points = getPoints();
    points.endPoint += delta;
    setPoints(points);
}

void Line::moveRadius(const int radius)
{
}

// Метод установки состояния выделения
void Line::setSelected(bool selected) {
    isSelected = selected;
}

void Line::setMoveSelected(bool selected)
{
    moveSelected = selected;
}

// Реализация метода проверки попадания точки в отрезок
bool Line::contains(const QPoint& point)  {
    const auto& points = getPoints();
    // Основная логика проверки принадлежности точки линии
    QPoint v1 = point - points.startPoint;
    QPoint v2 = points.endPoint - points.startPoint;

    double lengthSquared = v2.x() * v2.x() + v2.y() * v2.y();
    double dotProduct = v1.x() * v2.x() + v1.y() * v2.y();

    double projection = dotProduct / lengthSquared;

    if (projection < 0 || projection > 1) {
        return false;
    }

    QPoint closestPoint = points.startPoint + projection * v2;

    return (point - closestPoint).manhattanLength() < 5; // Допускаем небольшое расстояние для попадания
}

// Возвращает область квадрата в начале линии
// Возвращает область квадрата в начале линии
QRect Line::getStartHandle() const {
    const auto& points = getPoints();
    return handleManager->getHandle(HandleType::StartHandle, points.startPoint, points.endPoint);
}

QRect Line::getEndHandle() const {
    const auto& points = getPoints();
    return handleManager->getHandle(HandleType::EndHandle, points.startPoint, points.endPoint);
}

QRect Line::getMiddleHandle() const {
    const auto& points = getPoints();
    return handleManager->getHandle(HandleType::MiddleHandle, points.startPoint, points.endPoint);
}

QRect Line::getLeftHandle() const
{
    return QRect();
}

QRect Line::getToptHandle() const
{
    return QRect();
}

QRect Line::getRighttHandle() const
{
    return QRect();
}

QRect Line::getBottomtHandle() const
{
    return QRect();
}

bool Line::isHandleSelected(const HandleType& handle, const QPoint& point) const {
    const auto& points = getPoints();
    return handleManager->isHandleSelected(handle, point, points.startPoint, points.endPoint, 0);
}

void Line::captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const QPoint& startPoint, const QPoint& endPoint, int radius) 
{
    return handleManager->captureCursorForAllHandles(tabWidget, point,  startPoint, endPoint,radius);
}


HandleType Line::getHandleAt(const QPoint& point) {
    // Сбрасываем цвета хендлов
    ColorStartPoint = QColor(0, 127, 255);
    ColorEndPoint = QColor(0, 127, 255);
    ColorMiddlePoint = QColor(0, 127, 255);

    const auto& points = getPoints();
    // Проверка каждого хендла через HandleManager
    if (handleManager->isHandleSelected(HandleType::StartHandle, point, points.startPoint, points.endPoint, 0)) {
        if (isSelected) {
            ColorStartPoint = QColor(165, 0, 0);
        }
        setHandleState(HandleType::StartHandle);
        return HandleType::StartHandle;
    }

    if (handleManager->isHandleSelected(HandleType::EndHandle, point, points.startPoint, points.endPoint, 0)) {
        if (isSelected) {
            ColorEndPoint = QColor(165, 0, 0);
        }
        setHandleState(HandleType::EndHandle);
        return HandleType::EndHandle;
    }

    if (handleManager->isHandleSelected(HandleType::MiddleHandle, point, points.startPoint, points.endPoint, 0)) {
        if (isSelected) {
            ColorMiddlePoint = QColor(165, 0, 0);
        }
        setHandleState(HandleType::MiddleHandle);
        return HandleType::MiddleHandle;
    }

    // Если ни один хендл не выбран
    setHandleState(HandleType::None);
    return HandleType::None;
}


// Функция для установки состояния хендлов
void Line::setHandleState(HandleType handleType) {
    isStart = (handleType == HandleType::StartHandle);
    isEnd = (handleType == HandleType::EndHandle);
    isMiddle = (handleType == HandleType::MiddleHandle);
}


Circle::Circle(const QPoint& startPoint, const int radius)
  //  : startPoint(startPoint), radius(radius) 
  {
    handleManager = new HandleManager(10);
    ShapePoints points;
    points.startPoint = startPoint;
    points.radius = radius;
    setPoints(points);
}

Circle::Circle(const Circle& other)
   // : startPoint(other.startPoint), radius(other.radius) 
{
    handleManager = new HandleManager(10);
    const  ShapePoints& otherpoints = other.getPoints();
    ShapePoints points;
    points.startPoint = otherpoints.startPoint;
    points.radius = otherpoints.radius;
    setPoints(points);
}

Circle::~Circle()
{
    delete handleManager;
}


void Circle::draw(QPainter& painter) const
{
    const auto& points = getPoints();
    QPen pen = painter.pen();
    // Если линия выделена, изменяем цвет
    if (isSelected && !moveSelected) {
        QColor highlightedColor(90, 150, 255, 169);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawEllipse(points.startPoint, points.radius, points.radius);
        // возвращаем Pen
        painter.setPen(pen);
        // Рисуем квадраты на сверху снизу спарва и слева и по центру, если  выделено
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

    if (iscursorhovershape) {
        QColor highlightedColor(250, 250, 250, 120);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawEllipse(points.startPoint, points.radius, points.radius);

        // возвращаем Pen
        painter.setPen(pen);
    }
    painter.setPen(pen);
    painter.drawEllipse(points.startPoint, points.radius, points.radius);

   
}

void Circle::move(const QPoint& delta)
{
    auto points = getPoints();
    points.startPoint += delta; 
    setPoints(points);
}

void Circle::moveStart(const QPoint& delta)
{
    auto points = getPoints();
    points.startPoint += delta;
    setPoints(points);
}

void Circle::moveEnd(const QPoint& delta)
{

}

void Circle::moveRadius(const int radius_)
{
    auto points = getPoints();
    points.radius = radius_;
    setPoints(points);
}

bool Circle::contains(const QPoint& point) 
{
    const auto& points = getPoints();
    // Вычисляем расстояние от точки до центра
    double distanceSquared = pow(point.x() - points.startPoint.x(), 2) + pow(point.y() - points.startPoint.y(), 2);
    double radiusSquared = points.radius * points.radius;
    //float  result = fabs(distanceSquared - radiusSquared) ;
    // Проверяем условие на нахождение точки на окружности с учетом погрешности
    return fabs(distanceSquared - radiusSquared) < 1000;
}

void Circle::setSelected(bool selected)
{
    isSelected = selected;
}

void Circle::setMoveSelected(bool selected)
{
    moveSelected = selected;
}

HandleType Circle::getHandleAt(const QPoint& point)
{
    const auto& points = getPoints();
    // Сбрасываем цвета всех хендлов
    ColorStartPoint = QColor(0, 127, 255);
    ColorLeftPoint = QColor(0, 127, 255);
    ColorTopPoint = QColor(0, 127, 255);
    ColorRightPoint = QColor(0, 127, 255);
    ColorBottomPoint = QColor(0, 127, 255);

    // Проверка каждого хендла через HandleManager
    if (handleManager->isHandleSelected(HandleType::StartHandle, point, points.startPoint, QPoint(), points.radius)) {
        if (isSelected) {
            ColorStartPoint = QColor(165, 0, 0);
        }
        setHandleState(HandleType::StartHandle);
        return HandleType::StartHandle;
    }

    if (handleManager->isHandleSelected(HandleType::LeftHandle, point, points.startPoint, QPoint(), points.radius)) {
        if (isSelected) {
            ColorLeftPoint = QColor(165, 0, 0);
        }
        setHandleState(HandleType::LeftHandle);
        return HandleType::LeftHandle;
    }

    if (handleManager->isHandleSelected(HandleType::TopHandle, point, points.startPoint, QPoint(), points.radius)) {
        if (isSelected) {
            ColorTopPoint = QColor(165, 0, 0);
        }
        setHandleState(HandleType::TopHandle);
        return HandleType::TopHandle;
    }

    if (handleManager->isHandleSelected(HandleType::RightHandle, point, points.startPoint, QPoint(), points.radius)) {
        if (isSelected) {
            ColorRightPoint = QColor(165, 0, 0);
        }
        setHandleState(HandleType::RightHandle);
        return HandleType::RightHandle;
    }

    if (handleManager->isHandleSelected(HandleType::BottomHandle, point, points.startPoint, QPoint(), points.radius)) {
        if (isSelected) {
            ColorBottomPoint = QColor(165, 0, 0);
        }
        setHandleState(HandleType::BottomHandle);
        return HandleType::BottomHandle;
    }

    // Если ни один хендл не выбран
    setHandleState(HandleType::None);
    return HandleType::None;
}

// Функция для установки состояния хендлов
void Circle::setHandleState(HandleType handleType) {
    isStart = (handleType == HandleType::StartHandle);
    isLeft = (handleType == HandleType::LeftHandle);
    isTop = (handleType == HandleType::TopHandle);
    isRight = (handleType == HandleType::RightHandle);
    isBottom = (handleType == HandleType::BottomHandle);
}

//
std::shared_ptr<Shape> Circle::clone() const
{    
    return std::make_shared<Circle>(*this);
}

void Circle::setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected)
{
    
}

void Circle::setCentre(const QPoint& startPoint_, const int radius_, bool isSelected_)
{
    //startPoint = startPoint_;  radius = radius_; 
    isSelected = isSelected_;
    ShapePoints points;
    points.startPoint = startPoint_;
    points.radius = radius_;
    setPoints(points);
}

QPoint Circle::getstartPoint() const
{
    return getPoints().startPoint;
}

QPoint Circle::getendPoint() const
{
    return QPoint();
}

QPoint Circle::getmiddlePoint() const
{
    return QPoint();
}


int Circle::getradius() const
{
    return getPoints().radius;
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

void Circle::setisover(bool iscursorhovershape_)
{
    iscursorhovershape = iscursorhovershape_;
}

bool Circle::getisover() const
{
    return iscursorhovershape;
}

QRect Circle::getStartHandle() const {
    const auto& points = getPoints();
    return handleManager->getHandle(HandleType::StartHandle, points.startPoint, QPoint(), points.radius);
}


QRect Circle::getEndHandle() const
{
    return QRect();
}

QRect Circle::getMiddleHandle() const
{
    return QRect();
}

QRect Circle::getLeftHandle() const {
    const auto& points = getPoints();
    return handleManager->getHandle(HandleType::LeftHandle, points.startPoint, QPoint(), points.radius);
}

QRect Circle::getRighttHandle() const {
    const auto& points = getPoints();
    return handleManager->getHandle(HandleType::RightHandle, points.startPoint, QPoint(), points.radius);
}

QRect Circle::getToptHandle() const {
    const auto& points = getPoints();
    return handleManager->getHandle(HandleType::TopHandle, points.startPoint, QPoint(), points.radius);
}

QRect Circle::getBottomtHandle() const {
    const auto& points = getPoints();
    return handleManager->getHandle(HandleType::BottomHandle, points.startPoint, QPoint(), points.radius);
}

bool Circle::isHandleSelected(const HandleType& handle, const QPoint& point) const {
    const auto& points = getPoints();
    return handleManager->isHandleSelected(handle, point, points.startPoint, QPoint(), points.radius);
}

void Circle::captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const QPoint& startPoint, const QPoint& endPoint, int radius)
{
    return handleManager->captureCursorForAllHandles(tabWidget, point, startPoint, endPoint, radius);
}