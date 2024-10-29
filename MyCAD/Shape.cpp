#include "Shape.h"
#include <QDebug>
#include <QTabWidget>
#include <QWidget>

const ShapePoints& Shape::getPoints() const
{
    return points;
}

void Shape::setPoints(const ShapePoints& newPoints)
{
    points = newPoints;
}

void Shape::setHandleSize(int size)
{
    handleManager = new HandleManager(size);
}

HandleManager* Shape::getHandle() const
{
    return handleManager;
}

Shape::~Shape()
{
    delete handleManager;
}

Colors Shape::getColors() const
{
    return colors;
}

void Shape::setColorsStart(QColor color)
{
	colors.ColorStartPoint = color;

}

void Shape::setColorEnd(QColor color)
{
	colors.ColorEndPoint = color;
}

void Shape::setColorMiddle(QColor color)
{
	colors.ColorMiddlePoint = color;
}

void Shape::setColorsLeft(QColor color)
{
	colors.ColorLeftPoint = color;
}

void Shape::setColorsTop(QColor color)
{
	colors.ColorTopPoint = color;
}

void Shape::setColorsRight(QColor color)
{
	colors.ColorRightPoint = color;
}

void Shape::setColorsBottom(QColor color)
{
	colors.ColorBottomPoint = color;
}

Selected Shape::getSelected()  const
{
    return selected;
}


void Shape::setisEnd(bool selected)
{
}

void Shape::setisStart(bool isSelected)
{
    selected.isStart = isSelected;
}

void Shape::setisMiddle(bool isSelected)
{
    selected.isMiddle = isSelected;
}

void Shape::setisLeft(bool isSelected)
{
    selected.isLeft = isSelected;
}

void Shape::setisTop(bool isSelected)
{
    selected.isTop = isSelected;
}

void Shape::setisRight(bool isSelected)
{
    selected.isRight= isSelected;
}

void Shape::setisBottom(bool isSelected)
{
    selected.isBottom = isSelected;
}

void Shape::move(const QPoint& delta)
{
}

void Shape::moveStart(const QPoint& delta)
{
}

void Shape::moveEnd(const QPoint& delta)
{
}

void Shape::moveRadius(const int radius)
{
}

void Shape::setSelected(bool selected)
{
    isSelected = selected;
}

bool Shape::getisSelected() const
{
    return isSelected;
}

void Shape::setMoveSelected(bool selected)
{
    moveSelected = selected;
}

bool Shape::getisMoveSelected() const
{
    return moveSelected;
}

void Shape::resetColor()
{
    colors.ColorStartPoint = QColor(0, 127, 255);
    colors.ColorEndPoint = QColor(0, 127, 255);
    colors.ColorMiddlePoint = QColor(0, 127, 255);
    colors.ColorLeftPoint = QColor(0, 127, 255);
    colors.ColorTopPoint = QColor(0, 127, 255);
    colors.ColorRightPoint = QColor(0, 127, 255);
    colors.ColorBottomPoint = QColor(0, 127, 255);

    selected.isStart = false;  // Флаг выделения
    selected.isEnd = false;  // Флаг выделения
    selected.isMiddle = false;  // Флаг выделения 
    selected.isLeft = false;  // Флаг выделения
    selected.isTop = false;  // Флаг выделения
    selected.isRight = false;  // Флаг выделения
    selected.isBottom = false;  // Флаг выделения
}

bool Shape::getisStart() const
{
    return false;
}

bool Shape::getisEnd() const
{
    return false;
}

bool Shape::getisMiddle() const
{
    return false;
}

bool Shape::getisLeft() const
{
    return false;
}

bool Shape::getisRight() const
{
    return false;
}

bool Shape::getisTop() const
{
    return false;
}

bool Shape::getisBottom() const
{
    return false;
}

void Shape::setisover(bool iscursorhovershape_)
{
    iscursorhovershape = iscursorhovershape_;
}

bool Shape::getisover() const
{
    return iscursorhovershape;
}

QRect Shape::getStartHandle() const
{
    return QRect();
}

QRect Shape::getEndHandle() const
{
    return QRect();
}

QRect Shape::getMiddleHandle() const
{
    return QRect();
}

QRect Shape::getLeftHandle() const
{
    return QRect();
}

QRect Shape::getToptHandle() const
{
    return QRect();
}

QRect Shape::getRighttHandle() const
{
    return QRect();
}

QRect Shape::getBottomtHandle() const
{
    return QRect();
}

void Shape::setCoords(const QPoint& startPoint, const QPoint& endPoint, bool isSelected)
{
}

void Shape::setCentre(const QPoint& startPoint, const int radius, bool isSelected)
{
}

QPoint Shape::getstartPoint() const
{
    return QPoint();
}

QPoint Shape::getendPoint() const
{
    return QPoint();
}

QPoint Shape::getmiddlePoint() const
{
    return QPoint();
}

int Shape::getradius() const
{
    return 0;
}

void Shape::setcolorPoint() const
{
}

QColor Shape::getColorPoint() const
{
    return QColor();
}





// Реализация конструктора для класса Line
Line::Line(const QPoint& startPoint, const QPoint& endPoint)   
{
    setHandleSize(10);
    ShapePoints points;
    points.startPoint = startPoint;
    points.endPoint = endPoint;
    setPoints(points);
}

Line::Line(const Line& other)
{
    setHandleSize(10);
    const  ShapePoints& otherpoints = other.getPoints();
    ShapePoints points;
    points.startPoint = otherpoints.startPoint;
    points.endPoint = otherpoints.endPoint;
    setPoints(points);
}

// Реализация метода  клонирования
std::shared_ptr<Shape> Line::clone() const  {
    return std::make_shared<Line>(*this);
}

void Line::setCoords(const QPoint& startPoint_, const QPoint& endPoint_, bool isSelected_) 
{    
    setSelected(isSelected_);
    ShapePoints points;
    points.startPoint = startPoint_;
    points.endPoint = endPoint_;
    setPoints(points);
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
    return (points.startPoint + points.endPoint) / 2;
}


bool Line::getisStart() const
{
    return getSelected().isStart;
}

bool Line::getisEnd() const
{
    return getSelected().isEnd;
}

bool Line::getisMiddle() const
{
    return getSelected().isMiddle;
}

Type Line::name() const
{
    return Type::line;
}

// Реализация метода рисования отрезка
void Line::draw(QPainter& painter) const {
    QPen pen = painter.pen();
    const auto& points = getPoints();
    // Если линия выделена, изменяем цвет
    if (getisSelected() && !getisMoveSelected()) {
        QColor highlightedColor(90, 150, 255, 169);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawLine(points.startPoint, points.endPoint);
        // возвращаем Pen
        painter.setPen(pen);
        // Рисуем квадраты на концах и в середине, если линия выделена
        painter.setBrush(getColors().ColorStartPoint);
        painter.drawRect(getStartHandle());
        painter.setBrush(getColors().ColorEndPoint);
        painter.drawRect(getEndHandle());
        painter.setBrush(getColors().ColorMiddlePoint);
        painter.drawRect(getMiddleHandle());
        // Отключаем заливку
        painter.setBrush(Qt::NoBrush);
    }

     if (getisover()) {
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
QRect Line::getStartHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::StartHandle, points);
}

QRect Line::getEndHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::EndHandle, points);
}

QRect Line::getMiddleHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::MiddleHandle, points);
}

bool Line::isHandleSelected(const HandleType& handle, const QPoint& point) const {
    const auto& points = getPoints();
    return getHandle()->isHandleSelected(handle, point, points);
}

void Line::captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const ShapePoints& points)
{
     getHandle()->captureCursorForAllHandles(tabWidget, point,  points);
}


HandleType Line::getHandleAt(const QPoint& point) {
    // Сбрасываем цвета хендлов
    resetColor();

    const auto& points = getPoints();
    // Проверка каждого хендла через HandleManager
    if (getHandle()->isHandleSelected(HandleType::StartHandle, point, points)) {
        if (getisSelected()) {
            setColorsStart(QColor(165, 0, 0));
        }
        setHandleState(HandleType::StartHandle);
        return HandleType::StartHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::EndHandle, point, points)) {
        if (getisSelected()) {
            setColorEnd(QColor(165, 0, 0));
        }
        setHandleState(HandleType::EndHandle);
        return HandleType::EndHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::MiddleHandle, point, points)) {
        if (getisSelected()) {
            setColorMiddle(QColor(165, 0, 0));
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
    setisStart(handleType == HandleType::StartHandle);
    setisEnd(handleType == HandleType::EndHandle);
    setisMiddle(handleType == HandleType::MiddleHandle);
}


Circle::Circle(const QPoint& startPoint, const int radius)
  {
    setHandleSize(10);
    ShapePoints points;
    points.startPoint = startPoint;
    points.radius = radius;
    setPoints(points);
}

Circle::Circle(const Circle& other)
{
    setHandleSize(10);
    const  ShapePoints& otherpoints = other.getPoints();
    ShapePoints points;
    points.startPoint = otherpoints.startPoint;
    points.radius = otherpoints.radius;
    setPoints(points);
}

void Circle::draw(QPainter& painter) const
{
    const auto& points = getPoints();
    QPen pen = painter.pen();
    // Если линия выделена, изменяем цвет
    if (getisSelected() && !getisMoveSelected()) {
        QColor highlightedColor(90, 150, 255, 169);
        QPen highlightedPen(highlightedColor, 4, Qt::SolidLine);
        painter.setPen(highlightedPen);
        painter.drawEllipse(points.startPoint, points.radius, points.radius);
        // возвращаем Pen
        painter.setPen(pen);
        // Рисуем квадраты на сверху снизу спарва и слева и по центру, если  выделено
        painter.setBrush(getColors().ColorStartPoint);
        painter.drawRect(getStartHandle());

        painter.setBrush(getColors().ColorLeftPoint);
        painter.drawRect(getLeftHandle());

        painter.setBrush(getColors().ColorTopPoint);
        painter.drawRect(getToptHandle());

        painter.setBrush(getColors().ColorRightPoint);
        painter.drawRect(getRighttHandle());

        painter.setBrush(getColors().ColorBottomPoint);
        painter.drawRect(getBottomtHandle());

        // Отключаем заливку
        painter.setBrush(Qt::NoBrush);
    }

    if (getisover()) {
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
    // Проверяем условие на нахождение точки на окружности с учетом погрешности
    return fabs(distanceSquared - radiusSquared) < 1000;
}

HandleType Circle::getHandleAt(const QPoint& point)
{
    const auto& points = getPoints();
    // Сбрасываем цвета всех хендлов
    resetColor();

    // Проверка каждого хендла через HandleManager
    if (getHandle()->isHandleSelected(HandleType::StartHandle, point, points)) {
        if (getisSelected()) {
            setColorsStart(QColor(165, 0, 0));
        }
        setHandleState(HandleType::StartHandle);
        return HandleType::StartHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::LeftHandle, point, points)) {
        if (getisSelected()) {
            setColorsLeft(QColor(165, 0, 0));
        }
        setHandleState(HandleType::LeftHandle);
        return HandleType::LeftHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::TopHandle, point, points)) {
        if (getisSelected()) {
            setColorsTop(QColor(165, 0, 0));
        }
        setHandleState(HandleType::TopHandle);
        return HandleType::TopHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::RightHandle, point, points)) {
        if (getisSelected()) {
            setColorsRight(QColor(165, 0, 0));
        }
        setHandleState(HandleType::RightHandle);
        return HandleType::RightHandle;
    }

    if (getHandle()->isHandleSelected(HandleType::BottomHandle, point, points)) {
        if (getisSelected()) {
            setColorsBottom(QColor(165, 0, 0));
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
    setisStart(handleType == HandleType::StartHandle);
    setisLeft(handleType == HandleType::LeftHandle);
    setisTop(handleType == HandleType::TopHandle);
    setisRight(handleType == HandleType::RightHandle);
    setisBottom(handleType == HandleType::BottomHandle);
}

//
std::shared_ptr<Shape> Circle::clone() const
{    
    return std::make_shared<Circle>(*this);
}

void Circle::setCentre(const QPoint& startPoint_, const int radius_, bool isSelected_)
{   
    setSelected(isSelected_);
    ShapePoints points;
    points.startPoint = startPoint_;
    points.radius = radius_;
    setPoints(points);
}

QPoint Circle::getstartPoint() const
{
    return getPoints().startPoint;
}

int Circle::getradius() const
{
    return getPoints().radius;
}


bool Circle::getisStart()const
{
    return getSelected().isStart;
}

bool Circle::getisLeft()const
{
    return getSelected().isLeft;
}

bool Circle::getisRight()const
{
    return getSelected().isRight;
}

bool Circle::getisTop() const
{
    return getSelected().isTop;
}

bool Circle::getisBottom() const
{
    return getSelected().isBottom;
}

Type Circle::name() const
{
    return Type::circle;
}

QRect Circle::getStartHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::StartHandle, points);
}


QRect Circle::getLeftHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::LeftHandle, points);
}

QRect Circle::getRighttHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::RightHandle, points);
}

QRect Circle::getToptHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::TopHandle, points);
}

QRect Circle::getBottomtHandle() const {
    const auto& points = getPoints();
    return getHandle()->getHandle(HandleType::BottomHandle, points);
}

bool Circle::isHandleSelected(const HandleType& handle, const QPoint& point) const {
    const auto& points = getPoints();
    return getHandle()->isHandleSelected(handle, point, points);
}

void Circle::captureCursorForAllHandles(QTabWidget* tabWidget, const QPoint& point, const ShapePoints& points)
{
   getHandle()->captureCursorForAllHandles(tabWidget, point, points);
}
